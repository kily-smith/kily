

#include "cybermotor_ctrl.h"
#include <linux/can/raw.h>



CyberMotorCtrl::CyberMotorCtrl(int motor_canid_,const std::string& iface_)
{
    motor_canid = motor_canid_;
    iface = iface_;
    pos_=0;
    vel_=0;
    torque_=0;
    kp_=0;
    kd_=0;

    socket_ = socket(PF_CAN,SOCK_RAW,CAN_RAW);

    if(socket_<0){
        printf("[CyberMotorCtrl] create socket failed,errno=%d\n",errno);
        exit(-1);
    }

    strcpy(ifr.ifr_name,iface_.c_str());

    if(ioctl(socket_,SIOCGIFINDEX,&ifr) < 0)
    {
        printf("[CyberMotorCtrl] ioctl  failed,errno=%d\n",errno);
        exit(-1);
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(bind(socket_,(struct sockaddr *)&addr,sizeof(addr)) < 0)
    {
        printf("[CyberMotorCtrl] bind failed,errno=:%d\n",errno);
        close(socket_);
        exit(-1);
    }

    struct can_filter rfilter[1];
    rfilter[0].can_id = motor_canid_;
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(socket_, SOL_CAN_RAW, CAN_RAW_FILTER, rfilter, sizeof(rfilter));


    int ro = 0; // 0 表示关闭( 默认), 1 表示开启
    setsockopt(socket_, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &ro, sizeof(ro));

    flags = fcntl(socket_,F_GETFL,0);
    if(flags<0)
    {
        printf("[CyberMotorCtrl] fcntl : F_GETFL  failed,errno=%d\n",errno);
        close(socket_);
        exit(-1);
    }

    if(fcntl(socket_,F_SETFL,flags | O_NONBLOCK) < 0)
    {
        printf("[CyberMotorCtrl] fcntl : F_SETFL  failed,errno=%d\n",errno);
        close(socket_);
        exit(-1);
    }



    cmd_frame.can_id = motor_canid_;
    cmd_frame.can_dlc = 8;
    memset(cmd_frame.data,0xff,7);
    cmd_frame.data[7]=0xfd;

    cancel_frame.can_id = motor_canid_;
    cancel_frame.can_dlc = 8;
    memset(cancel_frame.data,0xff,7);
    cancel_frame.data[7]=0xfd;

    enable_frame.can_id = motor_canid_;
    enable_frame.can_dlc = 8;
    memset(enable_frame.data,0xff,7);
    enable_frame.data[7]=0xfc;

    setzero_frame.can_id = motor_canid_;
    setzero_frame.can_dlc = 8;
    memset(setzero_frame.data,0xff,7);
    setzero_frame.data[7]=0xfe;


    send_count = 0;
    cmd_count = 0;

    can_recv_thread =std::thread(&CyberMotorCtrl::can_recv_loop,this);

}
CyberMotorCtrl::~CyberMotorCtrl()
{
    continue_ = false;
    if(can_recv_thread.joinable())
    {
        can_recv_thread.join();
    }
    if(socket_)
    {
        close(socket_);
        socket_= 0;
    }
}

void CyberMotorCtrl::can_recv_loop()
{
    continue_ = true;
    ssize_t nbytes;
    while(continue_)
    {
        if(send_count<cmd_count){
            send_command();
            send_count++;
        }

        nbytes = read(socket_,&rcv_frame,sizeof(rcv_frame));
        if(nbytes<0)
        {
            if(errno!=EAGAIN){
                printf("[CyberMotorCtrl] read can failed,errno=%d\n",errno);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            else {
                //printf("[CyberMotorCtrl] recv again\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
        else if(nbytes < (ssize_t)sizeof(rcv_frame))
        {
            printf("[CyberMotorCtrl] read: incomplete BCM message\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else 
        {
            unpack_motor_msg(rcv_frame);
        }
    }
}
void CyberMotorCtrl::print_can_frame(const struct can_frame & frame)
{
    const unsigned char *data = frame.data;
    const unsigned int dlc = frame.can_dlc;
    unsigned int i;

    printf("%03X  [%u] ", frame.can_id, dlc);
    for (i = 0; i < dlc; ++i)
    {
        printf(" %02X", data[i]);
    }
    printf("\n");
}
void CyberMotorCtrl::unpack_motor_msg(const can_frame& frame)
{
    const unsigned int dlc = frame.can_dlc;
    const unsigned char* data=frame.data;
    //if(dlc==6){
    if((data[7]==0 && data[6==0]) || (data[6]==0xfe && data[7]==0xef)){
        int id = data[0];
        int p_int = data[1];
        p_int<<=8;
        p_int|=data[2];
        int v_int = data[3];
        v_int<<=4;
        v_int|=data[4]>>4;
        int t_int = data[4]&0xf;
        t_int<<=8;
        t_int|=data[5];

        cur_pos = uint_to_float(p_int, P_MIN, P_MAX, 16);
        cur_vel = uint_to_float(v_int, V_MIN, V_MAX, 12);
        cur_torque = uint_to_float(t_int, -I_MAX*KT*GR, I_MAX*KT*GR, 12);
        printf("[CyberMotorCtrl] RX Data: %02X %04X %03X %03X\n",id, p_int,v_int,t_int);
        printf("[CyberMotorCtrl] unpack_motor_msg: id=%03X, pos=% f, vel=% f, torque=% f\n",id,cur_pos,cur_vel,cur_torque);

        
    }
    /*else if(dlc==8){
        int p_int = (data[0]<<8)|(data[1]);
        int v_int = (data[2]<<4)|(data[3]>>4);
        int kp_int =((data[3]&0xf)<<8)|data[4];
        int kd_int =(data[5]<<4)|(data[6]>>4);
        int t_int=((data[6]&0xf)<<8)|data[7];

        float p = uint_to_float(p_int, P_MIN, P_MAX, 16);
        float v = uint_to_float(v_int, V_MIN, V_MAX, 12);
        float kp = uint_to_float(kp_int, KP_MIN, KP_MAX, 12);
        float kd = uint_to_float(kd_int, KD_MIN, KD_MAX, 12);
        float t = uint_to_float(t_int, -I_MAX*KT*GR, I_MAX*KT*GR, 12);

        //printf("[unpack_motor_msg] ------- from self\n");
        //printf("[unpack_motor_msg] ------- pos=% f, vel=% f, kp=% f, kd=% f, torque=% f\n",p,v,kp,kd,t);
    }*/
    else{
        printf("[CyberMotorCtrl][unpack_motor_msg] unknow data\n");
    }
}

void CyberMotorCtrl::send_command()
{
    int p_int = float_to_uint(pos_, P_MIN, P_MAX, 16);
    int v_int = float_to_uint(vel_, V_MIN, V_MAX, 12);
    int kp_int = float_to_uint(kp_, KP_MIN, KP_MAX, 12);
    int kd_int = float_to_uint(kd_, KD_MIN, KD_MAX, 12);
    int tq_int = float_to_uint(torque_, -I_MAX*KT*GR, I_MAX*KT*GR, 12);
    
    unsigned char* data=cmd_frame.data;
    data[0] = p_int >> 8;
    data[1] = p_int & 0xff;
    data[2] = v_int >> 4;
    data[3] = ((v_int & 0xf)<<4) | (kp_int>>8);
    data[4] = kp_int & 0xff;
    data[5] = kd_int >> 4;
    data[6] = ((kd_int & 0xf)<<4) | (tq_int>>8);
    data[7] = tq_int & 0xff;

    printf("[CyberMotorCtrl] set_motor, send_command: p:% f,v:% f,kp: %f,kd:% f,tq:% f\n",pos_,vel_,kp_,kd_,torque_);
    printf("[CyberMotorCtrl] TX Data: %02X %02X %02X %02X %02X %02X %02X %02X\n"
            ,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
    int bytes_sent = write(socket_,&cmd_frame,sizeof(cmd_frame));
    if(bytes_sent<0)
    {
        printf("[CyberMotorCtrl] send failed\n");
    }

}

void CyberMotorCtrl::set_motor(float pos,float vel,float kp,float kd,float torque)
{
    pos_=pos;
    vel_=vel;
    kp_=kp;
    kd_=kd;
    torque_=torque;
    cmd_count++;
}
void CyberMotorCtrl::stop()
{
    continue_=false;
}


bool CyberMotorCtrl::enable_motor()
{
    printf("[CyberMotorCtrl] enable_motor\n");
    int bytes_sent = write(socket_,&enable_frame,sizeof(enable_frame));
    if(bytes_sent<0)
    {
        printf("[CyberMotorCtrl] enable_motor failed\n");
        return false;
    }
    return true;
}
bool CyberMotorCtrl::cancel_motor()
{

    printf("[CyberMotorCtrl] cancel_motor\n");
    int bytes_sent = write(socket_,&cancel_frame,sizeof(cancel_frame));
    if(bytes_sent<0)
    {
        printf("[CyberMotorCtrl] cancel_motor failed\n");
        return false;
    }
    return true;
}
bool CyberMotorCtrl::set_zero_position()
{
    printf("[CyberMotorCtrl] set_zero_position\n");
    int bytes_sent = write(socket_,&setzero_frame,sizeof(setzero_frame));
    if(bytes_sent<0)
    {
        printf("[CyberMotorCtrl] set_zero_position failed\n");
        return false;
    }
    return true;
}

float CyberMotorCtrl::get_position()
{
    return cur_pos; 
}

