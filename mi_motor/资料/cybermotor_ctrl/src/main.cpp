#include "cybermotor_ctrl.h" 
#include <chrono>
#include <cmath>
#include <thread>
#include <signal.h>


//CyberMotorCtrl* cmc_ptr;
bool continue_ = true;

void signal_handle(int sig)
{
    if(sig==SIGINT){
        //cmc_ptr->cancel_motor();
        //cmc_ptr->stop();
        printf("you press ctrl+c\n");
        continue_ = false;
    }
};



int main(int argc,char** argv)
{
    signal(SIGINT,signal_handle);

    std::string can_name = "can0";
    int canid = 1;

    if(argc>1){
        can_name = std::string(argv[1]);
    }
    if(argc>2){
        canid = atoi(argv[2]);
        if(std::isnan(canid))
            canid =1;
        if(canid<0 || canid>127)
            canid = 1;

    }
    printf("use can: %s\n",can_name.c_str());


    CyberMotorCtrl cmc(canid,"can0");
    //cmc_ptr = &cmc;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    cmc.cancel_motor();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for(int i=0;i<1;i++)
    {
        cmc.enable_motor();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } 

    double angle = cmc.get_position();
    int i=0;
    while(i<10){
        cmc.cancel_motor();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cmc.enable_motor();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        angle = cmc.get_position();
        printf("angle:%f\n",angle);
        if(fabs(angle)<12.5)
            break;
        i++;
    }

    if(fabs(angle)>12.5)
        angle =0;

    double dx=0.1;
    while(continue_)
    {

            
        cmc.set_motor(angle, 0, 3, 1, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        angle += dx;

        if(angle>12)
            dx=-0.1;
        else if(angle<-12)
            dx=0.1;

    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    cmc.set_motor(0, 0, 0, 0, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    cmc.cancel_motor();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    cmc.stop();

};
