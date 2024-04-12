#include "MyMotorTool.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <limits>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <thread>
#include <filesystem>
#include <regex>

#include "math_ops.h"
#include "qcustomplot.h"

#include <QStandardItem>
#include <QHeaderView>

MyMotorTool::MyMotorTool(QWidget *parent ):QMainWindow(parent),
                                         ui(new Ui::MyMotorTool)
{
    ui->setupUi(this);
    QTimer *t_time = new QTimer(this);
    t_time->setTimerType(Qt::PreciseTimer);
    t_time->setInterval(0.01);//设置时间间隔为0.01秒
    t_time->start();
    QObject::connect(t_time, SIGNAL(timeout()), this, SLOT(time_out()));

    cfg_labels.push_back(ui->label_gr);
    cfg_labels.push_back(ui->label_kt);
    cfg_labels.push_back(ui->label_i_bw);
    cfg_labels.push_back(ui->label_i_max);
    cfg_labels.push_back(ui->label_p_max);
    cfg_labels.push_back(ui->label_v_max);
    cfg_labels.push_back(ui->label_kp_max);
    cfg_labels.push_back(ui->label_kd_max);
    cfg_labels.push_back(ui->label_i_fw_max);
    cfg_labels.push_back(ui->label_temp_max);
    cfg_labels.push_back(ui->label_i_max_cont);
    cfg_labels.push_back(ui->label_i_cal);
    cfg_labels.push_back(ui->label_can_id);
    cfg_labels.push_back(ui->label_master_id);
    cfg_labels.push_back(ui->label_can_timeout);

    double_spins.push_back(ui->spin_gr);
    double_spins.push_back(ui->spin_kt);
    double_spins.push_back(ui->spin_i_bw);
    double_spins.push_back(ui->spin_i_max);
    double_spins.push_back(ui->spin_p_max);
    double_spins.push_back(ui->spin_v_max);
    double_spins.push_back(ui->spin_kp_max);
    double_spins.push_back(ui->spin_kd_max);
    double_spins.push_back(ui->spin_i_fw_max);
    double_spins.push_back(ui->spin_temp_max);
    double_spins.push_back(ui->spin_i_max_cont);
    double_spins.push_back(ui->spin_i_cal);
    double_spins.push_back(ui->spin_can_id);
    double_spins.push_back(ui->spin_master_id);
    double_spins.push_back(ui->spin_can_timeout);


    
    ui->frame_setupmode->hide();
    ui->frame_motormode->hide();
    ui->frame_encodermode->hide();

    QStandardItemModel* m = new QStandardItemModel();
    m->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("TimeStamp")));
    m->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("ID")));
    m->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("Len")));
    m->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("Ascii")));
    m->setHorizontalHeaderItem(4,new QStandardItem(QObject::tr("Data")));
    ui->tab_msgs->setModel(m);
    /*
    ui->tab_msgs->setColumnWidth(0,120); 
    ui->tab_msgs->setColumnWidth(1,50); 
    ui->tab_msgs->setColumnWidth(2,60); 
    ui->tab_msgs->setColumnWidth(3,120); 
    ui->tab_msgs->setColumnWidth(4,360); 
    */
    ui->tab_msgs->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tab_msgs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    timer.start();

    create_plot_window();

    on_btn_refresh_clicked();

    //want_send_motor_control_frame = false;


}

void MyMotorTool::create_plot_window()
{
    dialog_plot = new QWidget();
    dialog_plot->setWindowTitle("show plot");
    dialog_plot->resize(1200,480);

    qplot = new QCustomPlot(dialog_plot);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(qplot,0,0);
    layout->setRowStretch(0, 1);
    layout->setColumnStretch(0, 1);
    dialog_plot->setLayout(layout);

    color[0] = Qt::darkGreen;
    color[1] = Qt::darkRed;
    color[2] = Qt::darkYellow;
    color[3] = Qt::blue;
    color[4] = Qt::red;
    color[5] = Qt::darkMagenta;
    color[6] = Qt::darkBlue;
    color[7] = Qt::darkCyan;
    color[8] = Qt::magenta;
    color[9] = Qt::cyan;
    color[10] = Qt::green;
    color[11] = Qt::yellow;
    color[12] = Qt::white;

    for(int i=0;i<3;i++)
    {
        qplot->addGraph();
        qplot->graph(i)->setPen(QPen(color[i]));
        qplot->graph(i)->setBrush(QBrush(QColor(0,0,255,20)));
    }

    qplot->xAxis2->setVisible(true);
    qplot->xAxis2->setTickLabels(false);
    qplot->yAxis2->setVisible(true);
    qplot->yAxis2->setTickLabels(false);
    qplot->yAxis->setRange(-21,21);
    qplot->yAxis2->setRange(-21,21);

    connect(qplot->xAxis,SIGNAL(rangeChanged(QCPRange)),qplot->xAxis2,SLOT(setRange(QCPRange)));
    connect(qplot->yAxis,SIGNAL(rangeChanged(QCPRange)),qplot->yAxis2,SLOT(setRange(QCPRange)));

    qplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}

void MyMotorTool::closeEvent(QCloseEvent *e)
{
    qDebug()<<"closeEvent";
    is_exit = true;
}
MyMotorTool::~MyMotorTool()
{
    if(this_can_id >=0){
        send_cmd(cmd_esc);
    }
    on_btn_stop_clicked();
    delete ui;
    is_exit= true;
    qDebug()<<"exit";

}



void MyMotorTool::on_btn_refresh_clicked()
{
    list_interfaces();
}
void MyMotorTool::on_btn_connect_clicked()
{
    //wait_motor_mode=false;
    if(socket_!=0){
        on_btn_stop_clicked();
    }
    QString iface = ui->cbx_interfaces->currentText();
    if(iface==""){
        QMessageBox::warning(this,"error","you must select a iface(example: can0) first!");
        return;
    }

    struct sockaddr_can addr;

    struct ifreq ifr;

    if(socket_==0){
        socket_ = socket(PF_CAN,SOCK_RAW,CAN_RAW);
        if(socket_<0){
            qDebug()<<"[on_btn_connect_clicked] create socket failed,errno="<<errno;
            return;
        }
        std::string s = iface.toStdString();

        strcpy(ifr.ifr_name,s.c_str());

        if(ioctl(socket_,SIOCGIFINDEX,&ifr) < 0)
        {
            qDebug()<<"[on_btn_connect_clicked] ioctl failed,errno="<<errno;
            return;
        }

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if(bind(socket_,(struct sockaddr *)&addr,sizeof(addr))<0)
        {
            qDebug()<<"[on_btn_connect_clicked] bind failed,errno="<<errno;
            ::close(socket_);
            return;
        }

        //struct can_filter rfilter[1];
        //rfilter[0].can_id = 3;
        //rfilter[0].can_mask = 0x3ff;

        //setsockopt(socket_,SOL_CAN_RAW,CAN_RAW_FILTER,rfilter,sizeof(rfilter));

        int ro=0;
        setsockopt(socket_, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &ro, sizeof(ro));

        int flags = fcntl(socket_,F_GETFL,0);
        if(flags<0)
        {
            qDebug()<<"[on_btn_connect_clicked] fcntl : F_GETFL failed,errno="<<errno;
            ::close(socket_);
            return;
        }

        if(fcntl(socket_,F_SETFL,flags | O_NONBLOCK)<0)
        {
            qDebug()<<"[on_btn_connect_clicked] fcntl : F_SETFL failed,errno="<<errno;
            ::close(socket_);
            return;
        }
        qDebug()<<"connected";
        connected = true;
        ui->label_connection_state->setText("Connected");
        ui->btn_stop->setEnabled(true);
        ui->btn_connect->setEnabled(false);
    }
}
void MyMotorTool::on_btn_clear_clicked()
{
    ui->tab_msgs->model()->removeRows(0,ui->tab_msgs->model()->rowCount());
}
void MyMotorTool::on_btn_stop_clicked()
{
    if(socket_){
        ::close(socket_);
        socket_ = 0;
        qDebug()<<"connect closed";
    }
    connected = false;
    ui->label_connection_state->setText("N/A");
    ui->btn_stop->setEnabled(false);
    ui->btn_connect->setEnabled(true);
}



void MyMotorTool::time_out()
{
    static int cnt=0;
    cnt++;
    recv_msg();


}

void MyMotorTool::recv_msg()
{
    int nbytes;

    can_frame rcv_frame;

    if(socket_)
    {
        nbytes = ::read(socket_,&rcv_frame,sizeof(rcv_frame));
        if(nbytes<0)
        {
            if(errno!=EAGAIN){
                qDebug()<<"[RECV] read can failed,errno="<<errno;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            else{
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

            }
        }
        else if(nbytes<(ssize_t)sizeof(rcv_frame))
        {
            qDebug()<<"[RECV] read: incomplete MSG";
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else{
            //in_recv_cfg=true;
            unpack_msg(rcv_frame); 
            //in_recv_cfg=false;
        }
        if(this_can_id!=-1 && !only_try_get_params){
            static int failed_cnt=0;
            if(failed_cnt>0) failed_cnt--;
            if((motors_data[this_can_id].state==MotorState::MotorMode) && failed_cnt<1)
            {
                if(ui->label_control_canframe->text()==""){
                    make_contorl_canframe();
                }else{
                    control_canframe.can_id = this_can_id;
                    control_canframe.can_dlc = 8;
                    nbytes = write(socket_,&control_canframe,sizeof(control_canframe));
                    if(nbytes<0)
                    {
                        qDebug()<<"[MyMotorTool] send motor control cmd failed";
                        failed_cnt=400;
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }else{
                        failed_cnt = 0;
                        //qDebug()<<"[MyMotorTool] send motor control cmd";
                    }

                }
                
            }
        }

    };

}
void MyMotorTool::unpack_motor_msg(const can_frame& frame,float &cur_pos,float& cur_vel,float& cur_torque)
{
    const unsigned int dlc = frame.can_dlc;
    const unsigned char* data=frame.data;
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

    float P_MAX = ui->spin_p_max->value();
    float P_MIN = -P_MAX;;
    float V_MAX = ui->spin_v_max->value();
    float V_MIN = - V_MAX;
    float KT = ui->spin_kt->value();
    float GR = ui->spin_gr->value();
    float I_MAX = ui->spin_i_max->value();

    cur_pos = uint_to_float(p_int, P_MIN, P_MAX, 16);
    cur_vel = uint_to_float(v_int, V_MIN, V_MAX, 12);
    cur_torque = uint_to_float(t_int, -I_MAX*KT*GR, I_MAX*KT*GR, 12);
}

void MyMotorTool::unpack_motor_msg(const can_frame& frame,float &angle,int& encoder_count)
{
    const unsigned int dlc = frame.can_dlc;
    const unsigned char* data=frame.data;
    int id = data[0];
    int p_int = data[2];
    p_int<<=8;
    p_int|=data[3];
    int enc_int = 0;
    enc_int += data[4];
    enc_int <<= 8;
    enc_int += data[5];
    enc_int <<=8;
    enc_int += data[6];


    angle = p_int;
    encoder_count = enc_int;
}
void MyMotorTool::unpack_motor_msg(const can_frame& frame,float &cur_pos,uint64_t& timestamp)
{
    const unsigned int dlc = frame.can_dlc;
    const unsigned char* data=frame.data;
    int id = data[0];
    int p_int = data[1];
    p_int<<=8;
    p_int|=data[2];
    uint32_t timestamp_int = 0;
    timestamp_int += data[3];
    timestamp_int <<= 8;
    timestamp_int += data[4];
    timestamp_int <<=8;
    timestamp_int += data[5];

    float P_MAX = ui->spin_p_max->value();
    float P_MIN = -P_MAX;;

    cur_pos = uint_to_float(p_int, P_MIN, P_MAX, 16);
    
    static uint32_t last_recv_timestamp = 0;
    int dt_ = timestamp_int - last_recv_timestamp;
    qDebug()<<timestamp_int<<", "<<dt_;
    if(dt_< 0)//来自电机的时间戳是24位的，微妙为单位,因此这个时间戳每隔一段时间会溢出从0开始，差值就会小于零
    {
        //判断是时间戳溢出还是时间错乱
        if(dt_ < -(1<<23)){
            //时间溢出
            dt_ += 1<<24;
            last_recv_timestamp = timestamp_int;
        }else{
            //时间错乱
            qDebug()<<"时间错乱，是否can消息收发被网络延误?";
            dt_ = 0;
        }
    }else{
        last_recv_timestamp = timestamp_int;
    }
    
    timestamp += dt_;

}



void MyMotorTool::unpack_msg(const can_frame& frame)
{
    const unsigned int dlc=frame.can_dlc;
    const unsigned char* data =frame.data;
    QStandardItemModel* model =(QStandardItemModel*) ui->tab_msgs->model();
    int row = model->rowCount();
    QStandardItem* item;
    QDateTime dt=QDateTime::currentDateTime();
    item=new QStandardItem(dt.toString("HH:mm:ss"));
    item->setTextAlignment(Qt::AlignHCenter);
    model->setItem(row,0,item);
    QString can_rx_id;
    can_rx_id.sprintf("%03X",frame.can_id);
    item=new QStandardItem(can_rx_id);
    item->setTextAlignment(Qt::AlignHCenter);
    model->setItem(row,1,item);
    item=new QStandardItem(QString::number(dlc));
    item->setTextAlignment(Qt::AlignHCenter);
    model->setItem(row,2,item);
    char buf[9];
    for(unsigned int i=0;i<dlc;i++){
        buf[i] = data[i];
    }
    buf[dlc]=0;
    QString asc(buf);
    model->setItem(row,3,new QStandardItem(asc));
    QString hex;
    hex.sprintf("%02X    %02X    %02X    %02X    %02X    %02X    %02X    %02X",
            data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
    model->setItem(row,4,new QStandardItem(hex));
    int id=data[0];
    MotorState oldstate;
    if(data[1]==0xFE && data[7]==0xEF){
        if(data[3]==0 && data[4]=='L' && data[5]=='Y' && data[6]=='J'){
            if(!motors_data.contains(id)){
                MotorData md;
                md.state = MotorState::MenuMode;
                motors_data[id] = md;
                QString canid;
                canid.sprintf("%03X",id);
                ui->cbx_select_motor_canid->addItem(canid);
                qDebug()<<"register id="<<id;
            }else{
                if(this_can_id==id){
                    if(motors_data[id].state!=MotorState::MenuMode){
                        motors_data[id].state=MotorState::MenuMode;
                        update_gui(id);
                    }
                }
            }

        }
        else if(data[2]=='S' && data[3]=='E' && data[4]=='T' && data[5]=='U' && data[6]=='P'){
            oldstate = motors_data[id].state;
            motors_data[id].state=MotorState::SetupMode;
            if(id == this_can_id)
            {
                update_gui(id);
            }
            qDebug()<<"enter setup mode: "<<id;
        }
        else if(data[2]=='C' && data[3]=='A' && data[4]=='L' && data[5]=='I' && data[6]=='B'){
            oldstate = motors_data[id].state;
            motors_data[id].state=MotorState::CalibrationMode;
            if(id == this_can_id)
            {
                update_gui(id);
            }
            qDebug()<<"enter calibration mode: "<<id;
        }
        else if(data[2]=='E' && data[3]=='N' && data[4]=='C' && data[5]==' ' && data[6]==' '){
            oldstate = motors_data[id].state;
            motors_data[id].state=MotorState::EncoderMode;
            if(id == this_can_id)
            {
                update_gui(id);
            }
            qDebug()<<"enter encoder mode: "<<id;
        }
        else if(data[2]=='M' && data[3]=='O' && data[4]=='T' && data[5]=='O' && data[6]=='R'){
            oldstate = motors_data[id].state;
            motors_data[id].state=MotorState::MotorMode;
            if(id == this_can_id)
            {
                update_gui(id);
            }
            send_cmd(cmd_nothing);
            qDebug()<<"enter motor mode: "<<id;
        }else {
            if(!motors_data.contains(id)){
                qDebug()<<"motors_data not contains this id: "<<id;
            }else{
                if(motors_data[id].state==MotorState::SetupMode)
                {
                    //qDebug() << "receive param ? id="<<id;
                    //参数解析
                    if(motors_data[id].config.unpack_config(data)){
                        //motors_data[id].state = MotorState::SetupMode;
                        if(id==this_can_id)
                        {
                            //update_gui(id);
                            update_setupmode_gui(id);
                        }

                    }else{
                        qDebug()<<"not a param ? : "<<data[0]<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<" "<<data[5]<<" "<<data[6]<<" "<<data[7];
                    }
                }
                else if(motors_data[id].state==MotorState::EncoderMode)
                {
                    float angle;
                    int encoder_count;
                    unpack_motor_msg(frame,angle,encoder_count);
                    motors_data[id].encoder_linelization_count = encoder_count;
                    motors_data[id].encoder_anglemulti = angle;
                    if(id==this_can_id){
                        // TODO 
                        qDebug() <<"canid: "<<id<<", angle_multi:: "<<angle<<"°, encoder linelization count: "<<encoder_count;
                        update_encodermode_gui(id);
                    }
                }
                else if(motors_data[id].state==MotorState::CalibrationMode)
                {


                }
                else{
                    qDebug()<<"unknow this msg: " <<data[0]<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<" "<<data[5]<<" "<<data[6]<<" "<<data[7];
                }
            }


        }
        return;
    }
    if(!motors_data.contains(id)){
        //qDebug()<<"not registered id: "<<id<<", return";
        return;
    }

    if(data[6]==0xFE && data[7]==0xef)
    {
        //MotorMode 

        oldstate = motors_data[id].state;
        float pos,vel,torque;
        unpack_motor_msg(frame, pos, vel, torque);
        if(oldstate!=MotorState::MotorMode && motors_data[id].wait_motor_mode)
        {
            motors_data[id].state=MotorState::MotorMode;
            qDebug()<<"MotorMode";
            motors_data[id].wait_motor_mode = false;
        }
        motors_data[id].pos = pos;
        motors_data[id].vel = vel;
        motors_data[id].torque = torque;
        motors_data[id].timestamp = 0;
        motors_data[id].encoder_msg_cnt++;

        if(id==this_can_id)
        {
            int64_t cur_time = timer.elapsed();
            double t = (double)cur_time * 1e-3;
            if(start_plot)
            {
                qplot->graph(0)->addData(t,(double)pos);
                qplot->graph(1)->addData(t,(double)vel);
                qplot->graph(2)->addData(t,(double)torque);
                last_plot_time = cur_time;
            }
            if(oldstate!=MotorState::MotorMode && motors_data[id].state==MotorState::MotorMode)
                update_gui(id);
            static int update_motormode_gui_cnt = 0;
            update_motormode_gui_cnt++;
            if(update_motormode_gui_cnt > 20){
                update_motormode_gui(id);
                double plot_offset = t - 10;
                plot_offset = plot_offset<0 ?0:plot_offset;
                qplot->xAxis->setRange(plot_offset,10,Qt::AlignLeft);
                qplot->replot();
            }
        }
        //qDebug()<<"receive pos vel torque, id="<<id;

    }else if(data[6]==0xFE && data[7]==0xcf){
        //MotorMode 

        oldstate = motors_data[id].state;
        float pos;
        unpack_motor_msg(frame, pos, timestamp);
        if(oldstate!=MotorState::MotorMode && motors_data[id].wait_motor_mode)
        {
            motors_data[id].state=MotorState::MotorMode;
            qDebug()<<"MotorMode";
            motors_data[id].wait_motor_mode = false;
        }
        motors_data[id].pos = pos;
        motors_data[id].vel = 0;
        motors_data[id].torque = 0;
        motors_data[id].timestamp = timestamp;
        motors_data[id].encoder_msg_cnt++;


        if(id==this_can_id)
        {
            double t = (double)timestamp * 1e-6;
            if(start_plot)
            {
                qplot->graph(0)->addData(t,(double)pos);
                qplot->graph(1)->addData(t,0);
                qplot->graph(2)->addData(t,0);
                last_plot_time = timestamp;
            }
            if(oldstate!=MotorState::MotorMode && motors_data[id].state==MotorState::MotorMode)
                update_gui(id);
            static uint64_t last_update_gui_timestamp = 0;
            if(timestamp - last_update_gui_timestamp > 50000){
                last_update_gui_timestamp = timestamp;
                update_motormode_gui(id);
                double plot_offset = t - 10;
                plot_offset = plot_offset<0 ?0:plot_offset;
                qplot->xAxis->setRange(plot_offset,10,Qt::AlignLeft);
                qplot->replot();
            }
            
        }
        //qDebug()<<"receive pos vel torque, id="<<id;

    }else{



    }


}

void MyMotorTool::update_encodermode_gui(int id)
{
    QString angle_txt,count_txt;
    angle_txt.sprintf("%f °C",motors_data[id].encoder_anglemulti);
    count_txt.sprintf("%d",motors_data[id].encoder_linelization_count);
    ui->label_encoder_anglemulti->setText(angle_txt);
    ui->label_encoder_linelization_count->setText(count_txt);
}
void MyMotorTool::update_setupmode_gui(int id)
{
    double m;
    auto& cfg = motors_data[id].config;

    m = cfg.KT * cfg.GR * cfg.I_MAX;
    ui->slider_torque->setMinimum(-m*10000);
    ui->slider_torque->setMaximum( m*10000);
    ui->spin_torque->setMaximum(m);
    ui->spin_torque->setMinimum(-m);

    ui->spin_gr->setValue(cfg.GR);
    ui->spin_kt->setValue(cfg.KT);
    ui->spin_i_bw->setValue(cfg.I_BW);
    ui->spin_i_max->setValue(cfg.I_MAX);
    ui->spin_p_max->setValue(cfg.P_MAX);
    ui->spin_v_max->setValue(cfg.V_MAX);
    ui->spin_kp_max->setValue(cfg.KP_MAX);
    ui->spin_kd_max->setValue(cfg.KD_MAX);
    ui->spin_i_fw_max->setValue(cfg.I_FW_MAX);
    ui->spin_temp_max->setValue(cfg.TEMP_MAX);
    ui->spin_i_max_cont->setValue(cfg.I_MAX_CONT);
    ui->spin_i_cal->setValue(cfg.I_CAL);
    ui->spin_can_id->setValue(cfg.CAN_ID);
    ui->spin_master_id->setValue(cfg.CAN_MASTER);
    ui->spin_can_timeout->setValue(cfg.CAN_TIMEOUT);

    ui->label_gr->setText(QString::number(cfg.GR)) ; 
    ui->label_kt->setText(QString::number(cfg.KT));
    ui->label_i_bw->setText(QString::number(cfg.I_BW)) ;
    ui->label_i_max->setText(QString::number(cfg.I_MAX)) ;
    ui->label_p_max->setText(QString::number(cfg.P_MAX)) ;
    ui->label_v_max->setText(QString::number(cfg.V_MAX)) ;
    ui->label_kp_max->setText(QString::number(cfg.KP_MAX)) ;
    ui->label_kd_max->setText(QString::number(cfg.KD_MAX)) ;
    ui->label_i_fw_max->setText(QString::number(cfg.I_FW_MAX)) ;
    ui->label_temp_max->setText(QString::number(cfg.TEMP_MAX)) ;
    ui->label_i_max_cont->setText(QString::number(cfg.I_MAX_CONT)) ;
    ui->label_i_cal->setText(QString::number(cfg.I_CAL)) ;
    ui->label_can_id->setText(QString::number(cfg.CAN_ID)) ;
    ui->label_master_id->setText(QString::number(cfg.CAN_MASTER)) ;
    ui->label_can_timeout->setText(QString::number(cfg.CAN_TIMEOUT)) ;


    ui->slider_pos->setMinimum(-cfg.P_MAX*10000);
    ui->slider_pos->setMaximum(cfg.P_MAX*10000);
    ui->spin_pos->setMaximum(cfg.P_MAX);
    ui->spin_pos->setMinimum(-cfg.P_MAX);

    ui->slider_vel->setMinimum(-cfg.V_MAX*10000);
    ui->slider_vel->setMaximum(cfg.V_MAX*10000);
    ui->spin_vel->setMaximum(cfg.V_MAX);
    ui->spin_vel->setMinimum(-cfg.V_MAX);

    ui->slider_kp->setMaximum(cfg.KP_MAX*10000);
    ui->slider_kp->setMinimum(0);
    ui->spin_kp->setMaximum(cfg.KP_MAX);
    ui->spin_kp->setMinimum(0);

    ui->slider_kd->setMaximum(cfg.KD_MAX*10000);
    ui->slider_kd->setMinimum(0);
    ui->spin_kd->setMaximum(cfg.KD_MAX);
    ui->spin_kd->setMinimum(0);
}
void MyMotorTool::update_motormode_gui(int id)
{
    QString pos_txt,vel_txt,tq_txt,timestamp_txt;
    pos_txt.sprintf("% .6f",motors_data[id].pos);
    vel_txt.sprintf("% .6f",motors_data[id].vel);
    tq_txt.sprintf("% .6f",motors_data[id].torque);
    timestamp_txt.sprintf("%ld",motors_data[id].timestamp);
    ui->label_pos->setText(pos_txt);
    ui->label_vel->setText(vel_txt);
    ui->label_torque->setText(tq_txt);
    ui->label_timestamp->setText(timestamp_txt);


    if(motors_data[id].encoder_msg_cnt==1){
        motormode_set_desired_pos(motors_data[id].pos);
        motormode_set_desired_vel(0);
        motormode_set_desired_torque(0);            
        return;
    }
}

void MyMotorTool::update_gui(int id)
{
    switch(motors_data[id].state)
    {
        case MotorState::Dead:
            ui->btn_calibration_encoder->setEnabled(false);
            ui->btn_set_zero->setEnabled(false);
            ui->btn_encoder_mode->setEnabled(false);
            ui->btn_setup_mode->setEnabled(false);
            ui->btn_motor_mode->setEnabled(false);
            ui->frame_setupmode->hide();
            ui->frame_motormode->hide();
            ui->frame_encodermode->hide();
            break;
        case MotorState::MenuMode:
        case MotorState::Connected:
            ui->btn_calibration_encoder->setEnabled(true);
            ui->btn_set_zero->setEnabled(true);
            ui->btn_encoder_mode->setEnabled(true);
            ui->btn_setup_mode->setEnabled(true);
            ui->btn_motor_mode->setEnabled(true);
            ui->frame_setupmode->hide();
            ui->frame_motormode->hide();
            ui->frame_encodermode->hide();
            break;

        case MotorState::CalibrationMode:
            ui->btn_calibration_encoder->setEnabled(false);
            ui->btn_set_zero->setEnabled(false);
            ui->btn_encoder_mode->setEnabled(false);
            ui->btn_setup_mode->setEnabled(false);
            ui->btn_motor_mode->setEnabled(false);
            ui->frame_setupmode->hide();
            ui->frame_motormode->hide();
            ui->frame_encodermode->hide();
            break;

        case MotorState::SetupMode:
            ui->btn_calibration_encoder->setEnabled(false);
            ui->btn_set_zero->setEnabled(false);
            ui->btn_encoder_mode->setEnabled(false);
            ui->btn_setup_mode->setEnabled(false);
            ui->btn_motor_mode->setEnabled(false);
            //if(motors_data[id].config.motors_param_geted==32767)
                ui->frame_setupmode->show();
            ui->frame_motormode->hide();
            ui->frame_encodermode->hide();
            break;
        case MotorState::MotorMode:
            ui->btn_calibration_encoder->setEnabled(false);
            ui->btn_set_zero->setEnabled(false);
            ui->btn_encoder_mode->setEnabled(false);
            ui->btn_setup_mode->setEnabled(false);
            ui->btn_motor_mode->setEnabled(false);
            ui->frame_setupmode->hide();
            ui->frame_motormode->show();
            ui->frame_encodermode->hide();
            break;
        case MotorState::EncoderMode:
            ui->btn_calibration_encoder->setEnabled(false);
            ui->btn_set_zero->setEnabled(false);
            ui->btn_encoder_mode->setEnabled(false);
            ui->btn_setup_mode->setEnabled(false);
            ui->btn_motor_mode->setEnabled(false);
            ui->frame_setupmode->hide();
            ui->frame_motormode->hide();
            ui->frame_encodermode->show();
            break;


    }

}


void MyMotorTool::list_interfaces()
{
    const std::regex interface_regex(".*\\/(v?can\\d+)");
    const std::string sysfs_dir("/sys/class/net");
    QList<QString> interfaces;
    for(const auto& it: std::filesystem::directory_iterator(sysfs_dir))
    {
        std::string path=it.path().string();
        std::smatch mathces;
        if(std::regex_search(path,mathces,interface_regex)){
            QString s = QString::fromStdString(mathces[1].str());
            interfaces.push_back(s);
            qDebug() << "[list_interfaces] find interface: " << s;
        }
    }
    ui->cbx_interfaces->clear();
    ui->cbx_interfaces->addItems(interfaces);
    if(interfaces.size()==0){
        qDebug()<<"[list_interfaces] not found any SocketCAN interface";
    }
}

void MyMotorTool::on_btn_exit_encoder_mode_clicked()
{
    if(this_can_id==-1 )
    {
        return;
    }
    if(! socket_){
        return;
    }
    send_cmd(cmd_esc);     
}
void MyMotorTool::on_btn_exit_setup_mode_clicked()
{
    if(this_can_id==-1 )
    {
        return;
    }
    if(! socket_){
        return;
    }
    send_cmd(cmd_esc);     
}
void MyMotorTool::on_btn_exit_motor_mode_clicked()
{
    //want_send_motor_control_frame = false;
    if(this_can_id==-1 )
    {
        return;
    }
    if(! socket_){
        return;
    }
    send_cmd(cmd_esc);     
}
void MyMotorTool::on_btn_show_plot_clicked()
{
    //TODO
    dialog_plot->show();
    QPoint gp(dialog_plot->mapToGlobal(QPoint(0,0)));
    qDebug()<<gp;
    dialog_plot->move(gp.x(),gp.y());
    for(int i=0;i<3;++i)
        qplot->graph(i)->data()->removeBefore(last_plot_time+1);
    qplot->replot();
    start_plot = true;
}
void MyMotorTool::on_btn_copy_from_motor_clicked()
{
    int double_sz = double_spins.size();
    for(int i=0;i<double_sz;i++)
    {
        double v=cfg_labels[i]->text().toFloat();
        double_spins[i]->setValue(v);
    }
}
void MyMotorTool::on_btn_write_config_clicked()
{
    if(this_can_id==-1 )
    {
        QMessageBox::warning(this,"error","you must select a can_id first!");
        return;
    }
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }
    int double_sz = double_spins.size();
    //TODO   
    unsigned char buf[8];
    int update_cnt=0;
    for(int i=0;i<double_sz;i++){
        double v=double_spins[i]->value();
        QString old = cfg_labels[i]->text();
        double oldv = old.toFloat();
        double e=v-oldv;
        if(e>1e-6 || e<-1e-6)
        {
            buf[0] = 255;
            buf[1] = 255;    
            buf[2] = setup_cmds[i];
            pack_float(v, &buf[3]);
            buf[7] = 255;
            send_cmd(buf);
            char name_[] = {setup_cmds[i],0};
            qDebug()<<"update "<<QString(name_)<<" from "<<old<<" to "<<v;
            update_cnt++;
        }
    }
    if(update_cnt==0){
        qDebug()<<"nothing to do! all value is not change!";
    }

    
}
void MyMotorTool::on_btn_calibration_encoder_clicked()
{
    if(this_can_id==-1 )
    {
        QMessageBox::warning(this,"error","you must select a can_id first!");
        return;
    }
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }
    motors_data[this_can_id].wait_motor_mode = false;
    send_cmd(cmd_esc);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    send_cmd(cmd_cal);     
}
void MyMotorTool::on_btn_set_zero_clicked()
{
    if(this_can_id==-1 )
    {
        QMessageBox::warning(this,"error","you must select a can_id first!");
        return;
    }
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }
    send_cmd(cmd_zero);     
    motors_data[this_can_id].wait_motor_mode = false;
}
void MyMotorTool::on_btn_encoder_mode_clicked()
{
    if(this_can_id==-1 )
    {
        QMessageBox::warning(this,"error","you must select a can_id first!");
        return;
    }
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }
    motors_data[this_can_id].wait_motor_mode = false;
    send_cmd(cmd_esc);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    send_cmd(cmd_enc);     
}
void MyMotorTool::on_btn_setup_mode_clicked()
{
    if(this_can_id==-1 )
    {
        QMessageBox::warning(this,"error","you must select a can_id first!");
        return;
    }
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }
    motors_data[this_can_id].wait_motor_mode = false;
    int flag = motors_data[this_can_id].config.motors_param_geted & 32767;

    only_try_get_params=true;
    qDebug()<<"get param ...."; 
    send_cmd(cmd_esc);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    send_cmd(cmd_setup);
    int cnt=0;
    while(flag!=32767)
    {
        recv_msg();
        cnt++;
        if(cnt>60){
            cnt=0;
            send_cmd(cmd_nothing);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        flag = motors_data[this_can_id].config.motors_param_geted & 32767;

        qDebug() << "get param flag: "<< flag;
    }



    std::this_thread::sleep_for(std::chrono::microseconds(10));
}
void MyMotorTool::on_btn_motor_mode_clicked()
{
    
    if(this_can_id==-1 )
    {
        QMessageBox::warning(this,"error","you must select a can_id first!");
        return;
    }
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }

    int flag = motors_data[this_can_id].config.motors_param_geted & 32767;
    if(flag !=32767){
        only_try_get_params=true;
        qDebug()<<"get param ...."; 
        send_cmd(cmd_esc);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        send_cmd(cmd_setup);
        int cnt=0;
        while(flag!=32767)
        {
            recv_msg();
            cnt++;
            if(cnt>60){
                cnt=0;
                send_cmd(cmd_nothing);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            flag = motors_data[this_can_id].config.motors_param_geted & 32767;

            qDebug() << "get param flag: "<< flag;
        }
    }

    for(int i=0;i<3;i++){
        send_cmd(cmd_nothing);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    send_cmd(cmd_esc);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    motors_data[this_can_id].wait_motor_mode = true;
    send_cmd(cmd_motor);     
    only_try_get_params=false;
    //want_send_motor_control_frame = true;

}
void MyMotorTool::on_btn_select_clicked()
{
    if(! socket_){
        QMessageBox::warning(this,"error","you must create a SocketCAN connection first!");
        return;
    }
    QString canidtxt = ui->edt_custom_motor_canid->text();
    bool flag=true;
    int id;
    if(canidtxt==""){
        flag=false;
    }
    else{
        bool ok;
        id = canidtxt.toInt(&ok,16);
        if(!ok){
            flag=false;
        }
        else if(flag){
            if(id<0 || id>127){
                flag=false;
            }
        }

    }
    if(flag){
        //当前电机执行退出命令
        send_cmd(cmd_esc);
        //设置新的电机id
        this_can_id = id;
        //新电机执行退出命令
        send_cmd(cmd_esc);     
        //
        motors_data[this_can_id].wait_motor_mode = false;
        qDebug()<<"selec motor by canid "<<this_can_id<<" successful!";
    }else{
        QMessageBox::warning(this,"error","canid must be hex number between 0x00 - 0x7f");
        ui->edt_custom_motor_canid->setText("");
    }
}

void MyMotorTool::on_cbx_reply_content_currentIndexChanged(const QString& item_name)
{
    if(this_can_id==-1 )
    {
        return;
    }
    if(! socket_){
        return;
    }
    static QString last_select = QString("");
    if(last_select!=item_name){
        qDebug()<<"reply content: "<<item_name;

        if(item_name==QString("Pos_TimeStamp")){
            send_cmd(cmd_set_reply_pos_timestamp);
        }
        else if(item_name==QString("Pos_Vel_Torque")){
            send_cmd(cmd_set_reply_pos_vel_torque);
        }
        else{
            
        }
    }
    last_select = item_name;
}
void MyMotorTool::on_cbx_select_motor_canid_currentIndexChanged(const QString& arg1 )
{
    QString canid=ui->cbx_select_motor_canid->currentText();
    ui->edt_custom_motor_canid->setText(canid);
}
void MyMotorTool::send_cmd(const unsigned char* data)
{
    if(this_can_id==-1 )
    {
        return;
    }
    if(! socket_){
        return;
    }

    can_frame f;

    f.can_id = this_can_id;
    f.can_dlc = 8;
    for(int i=0;i<8;i++)
        f.data[i]=data[i];

    int bytes_sent=::write(socket_,&f,sizeof(f));
    if(bytes_sent<0)
    {
        printf("send_cmd failed! %2x %2x %2x %2x %2x %2x %2x %2x\n",
                data[0],
                data[1],
                data[2],
                data[3],
                data[4],
                data[5],
                data[6],
                data[7]
                );
        //QMessageBox::warning(this,"error","send_cmd failed");
    }

}
void MyMotorTool::motormode_set_desired_pos(float pos)
{
    ui->spin_pos->setValue(pos);
}
void MyMotorTool::motormode_set_desired_vel(float vel)
{
    ui->spin_vel->setValue(vel);
}
void MyMotorTool::motormode_set_desired_torque(float torque)
{
    ui->spin_torque->setValue(torque);
}
void MyMotorTool::make_contorl_canframe()
{
    if(desired_cmd_changed_flag==0 || this_can_id==-1)return;

    int flag = motors_data[this_can_id].config.motors_param_geted & 32767;
    if(flag !=32767){
        qDebug()<<"warning! please get param first! param_flag="<<flag; 

        return;
    }
    if(motors_data[this_can_id].encoder_msg_cnt<1){
        return;
    }


    float P_MAX = ui->spin_p_max->value();
    float P_MIN = -P_MAX;;
    float V_MAX = ui->spin_v_max->value();
    float V_MIN = - V_MAX;
    float KT = ui->spin_kt->value();
    float GR = ui->spin_gr->value();
    float I_MAX = ui->spin_i_max->value();
    float KD_MAX = ui->spin_kd_max->value();
    float KD_MIN = 0;
    float KP_MAX = ui->spin_kp_max->value();
    float KP_MIN = 0;

    int p_int = float_to_uint(ui->spin_pos->value(), P_MIN, P_MAX, 16);
    int v_int = float_to_uint(ui->spin_vel->value(), V_MIN, V_MAX, 12);
    int kp_int = float_to_uint(ui->spin_kp->value(), KP_MIN, KP_MAX, 12);
    int kd_int = float_to_uint(ui->spin_kd->value(), KD_MIN, KD_MAX, 12);
    int tq_int = float_to_uint(ui->spin_torque->value(), -I_MAX*KT*GR, I_MAX*KT*GR, 12);
    
    unsigned char* data = control_canframe.data;
    data[0] = p_int >> 8;
    data[1] = p_int & 0xff;
    data[2] = v_int >> 4;
    data[3] = ((v_int & 0xf)<<4) | (kp_int>>8);
    data[4] = kp_int & 0xff;
    data[5] = kd_int >> 4;
    data[6] = ((kd_int & 0xf)<<4) | (tq_int>>8);
    data[7] = tq_int & 0xff;

    QString txt;
    txt.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X\n"
            ,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
    ui->label_control_canframe->setText(txt);


    desired_cmd_changed_flag = 0;
}


void MyMotorTool::on_spin_pos_valueChanged(double v){
    if(valueChange(ui->slider_pos,v)){
        desired_cmd_changed_flag|=1;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_slider_pos_valueChanged(int vv){
    if(valueChange(ui->slider_pos,ui->spin_pos)){
        desired_cmd_changed_flag|=1;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_spin_vel_valueChanged(double v){
    if(valueChange(ui->slider_vel,v)){
        desired_cmd_changed_flag|=2;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_slider_vel_valueChanged(int vv){
    if(valueChange(ui->slider_vel,ui->spin_vel)){
        desired_cmd_changed_flag|=2;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_spin_kp_valueChanged(double v){
    if(valueChange(ui->slider_kp,v)){
        desired_cmd_changed_flag|=4;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_slider_kp_valueChanged(int vv){
    if(valueChange(ui->slider_kp,ui->spin_kp)){
        desired_cmd_changed_flag|=4;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_spin_kd_valueChanged(double v){
    if(valueChange(ui->slider_kd,v)){
        desired_cmd_changed_flag|=8;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_slider_kd_valueChanged(int vv){
    if(valueChange(ui->slider_kd,ui->spin_kd)){
        desired_cmd_changed_flag|=8;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_spin_torque_valueChanged(double v){
    if(valueChange(ui->slider_torque,v)){
        desired_cmd_changed_flag|=16;
        make_contorl_canframe();
    }
}
void MyMotorTool::on_slider_torque_valueChanged(int vv){
    if(valueChange(ui->slider_torque,ui->spin_torque)){
        desired_cmd_changed_flag|=16;
        make_contorl_canframe();
    }
}
bool MyMotorTool::valueChange(QSlider* slider,float v)
{
    int V = v*10000;
    if(v==V){
        return false;
    }
    slider->setValue(V);
    return true;
}
bool MyMotorTool::valueChange(QSlider* slider,QDoubleSpinBox* spin)
{
    float v = slider->value()*0.0001;
    float e=spin->value()-v;
    if(e>1e-6 || e<-1e-6){
        spin->setValue(v);
        return true;
    }else{
        return false;
    }
}
void MyMotorTool::make_zero_contorl_canframe()
{
    int p_int = float_to_uint(0, -12.5, 12.5, 16);
    int v_int = float_to_uint(0, -20.5, 20.5, 12);
    int kp_int = float_to_uint(0, 0, 100, 12);
    int kd_int = float_to_uint(0, 0, 5, 12);
    int tq_int = float_to_uint(0, -24, 24, 12);
    
    unsigned char* data = control_canframe.data;
    data[0] = p_int >> 8;
    data[1] = p_int & 0xff;
    data[2] = v_int >> 4;
    data[3] = ((v_int & 0xf)<<4) | (kp_int>>8);
    data[4] = kp_int & 0xff;
    data[5] = kd_int >> 4;
    data[6] = ((kd_int & 0xf)<<4) | (tq_int>>8);
    data[7] = tq_int & 0xff;

    QString txt;
    txt.sprintf("%02X %02X %02X %02X %02X %02X %02X %02X\n"
            ,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
    ui->label_control_canframe->setText(txt);
    qDebug()<<"zero control: "<<txt;
}
