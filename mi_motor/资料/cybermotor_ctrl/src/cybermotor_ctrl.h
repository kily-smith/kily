#ifndef _CYBERMOTOR_CTRL_H_
#define _CYBERMOTOR_CTRL_H_ 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <fcntl.h> 
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <cstring>

#include "math_ops.h"



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


class CyberMotorCtrl 
{
    private:
        int motor_canid;
        std::string iface;
        struct sockaddr_can addr;
        struct ifreq ifr;
        int socket_;
        int flags;
        can_frame rcv_frame;
        can_frame cmd_frame;
        can_frame enable_frame;
        can_frame cancel_frame;
        can_frame setzero_frame;
        std::thread can_recv_thread;
        bool continue_;
        float pos_,vel_,kp_,kd_,torque_;
        uint32_t send_count;
        uint32_t cmd_count;
        float cur_pos=1000,cur_vel,cur_torque;
    public:
        CyberMotorCtrl(int motor_canid_,const std::string& iface_);
        ~CyberMotorCtrl();

        void stop();
        void set_motor(float pos,float vel,float kp,float kd,float torque);
        bool enable_motor();
        bool cancel_motor();
        bool set_zero_position();
        float get_position();
    private:
        void can_recv_loop();
        inline void print_can_frame(const struct can_frame &  frame);

        inline void unpack_motor_msg(const can_frame& frame);

        inline void send_command();

};


#endif
