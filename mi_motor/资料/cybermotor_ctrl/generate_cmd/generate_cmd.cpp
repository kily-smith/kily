
#include "math_ops.h"

#include <iostream>


#define MSGID (0x0BC)
#define P_MIN -12.5
#define P_MAX  12.5 
#define V_MIN  -20.5
#define V_MAX   20.5
#define KP_MIN  0.0 
#define KP_MAX  100.0 
#define KD_MIN  0.0 
#define KD_MAX  5.0
#define I_MAX   40.0 
#define KT      0.1 
#define GR      6.0

int main(int argc,char** argv)
{
    std::cout << "usage: " << std::endl;
    std::cout << argv[0] << " pos vel kp kd torque"<<std::endl;
    std::cout <<std::endl;
    std::cout <<"enter motor mod: fffffffffffffffc"<<std::endl;
    std::cout <<"set zero pos: fffffffffffffffe"<<std::endl;
    std::cout <<"enter setup mode: fffffffffffffffb"<<std::endl;
    std::cout <<"enter calibrate mode: fffffffffffffffa"<<std::endl;
    std::cout <<"enter encode mode: fffffffffffffff9"<<std::endl;
    std::cout <<"return to menu mode: fffffffffffffffd"<<std::endl;
    std::cout <<"cmd_nothing: ffffffffffffff01"<<std::endl;
    std::cout <<std::endl;

    float pos = 0;
    float vel = 0;
    float kp = 0;
    float kd = 0;
    float tq = 0;

    if(argc>1) 
        pos = atof(argv[1]);
    if(argc>2)
        vel = atof(argv[2]);
    if(argc>3)
        kp = atof(argv[3]);
    if(argc>4)
        kd = atof(argv[4]);
    if(argc>5)
        tq = atof(argv[5]);

    std::cout << "pos: "<<pos<<" (rad), vel: "<<vel<<" (rad/second)"<<std::endl;
    std::cout << "kp: "<<kp<<", kd: "<<kd<<std::endl;
    std::cout << "desired torque: "<<tq<<" (N.m)"<<std::endl;
    std::cout << std::endl;

    int p_int = float_to_uint(pos, P_MIN, P_MAX, 16);
    int v_int = float_to_uint(vel, V_MIN, V_MAX, 12);
    int kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
    int kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);
    int tq_int = float_to_uint(tq, -I_MAX*KT*GR, I_MAX*KT*GR, 12);

    unsigned char data[8];
    data[0] = p_int >> 8;
    data[1] = p_int & 0xff;
    data[2] = v_int >> 4;
    data[3] = ((v_int & 0xf)<<4) | (kp_int>>8);
    data[4] = kp_int & 0xff;
    data[5] = kd_int >> 4;
    data[6] = ((kd_int & 0xf)<<4) | (tq_int>>8);
    data[7] = tq_int & 0xff;



    printf("generate can_frame: %02X %02X %02X %02X %02X %02X %02X %02X\n"
            ,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);



}
