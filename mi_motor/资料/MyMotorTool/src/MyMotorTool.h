#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MyMotorTool.h"
#include <QMainWindow>
#include <QObject>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <cstdint>
#include <qdebug.h>
#include <QEventLoop>
#include <QCloseEvent>
#include <fstream>
#include <QMessageBox>

#include "qcustomplot.h"

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <thread>
#include <pthread.h>
#include <vector>



namespace Ui { class MyMotorTool; }

enum MotorState{
    Dead=0,
    Connected ,
    MenuMode,
    SetupMode,
    CalibrationMode,
    EncoderMode,
    MotorMode

};

union FloatBytes {
    float value;
    unsigned char bytes[4];
};

static float unpack_float(const unsigned char* data)
{
    FloatBytes fb;
    fb.bytes[0] = data[0];
    fb.bytes[1] = data[1];
    fb.bytes[2] = data[2];
    fb.bytes[3] = data[3];
    return fb.value;
}
static void pack_float(float v, unsigned char* data)
{
    FloatBytes fb;
    fb.value = v;
    data[0] = fb.bytes[0];
    data[1] = fb.bytes[1];
    data[2] = fb.bytes[2];
    data[3] = fb.bytes[3];
}

struct MotorConfig{
    double GR=6;
    double KT=0.1;
    double I_BW=1000.0;
    double I_MAX=40.0;
    double P_MAX=12.5;
    double V_MAX=20.5;
    double KP_MAX=500.0;
    double KD_MAX=5.0;
    double I_FW_MAX=33.0;
    double TEMP_MAX=8.0;
    double I_MAX_CONT=15.0;
    double I_CAL=5.0;
    int CAN_ID=1;
    int CAN_MASTER=0;
    int CAN_TIMEOUT=20000;


    int motors_param_geted=0;

    bool unpack_config(const unsigned char* data)
    {
        int id = (int)data[0];        
        //if(id!=CAN_ID)
        //    return;
#if 0
        char numstr[7];
        for(int i=0;i<6;i++)
            numstr[i]=data[i+2];
        numstr[6]=0;
        QString num(numstr);
        float fn = num.toFloat();
#else 
        float fn = unpack_float(data+3);
#endif
        
        switch(data[2])
        {
            case 'g':
                GR = fn;
                motors_param_geted|=1;
                break;
            case 'k':
                KT = fn;
                motors_param_geted|=2;
                break;
            case 'b':
                I_BW = fn;
                motors_param_geted|=4;
                break;
            case 'l':
                I_MAX = fn;
                motors_param_geted|=8;
                break;
            case 'p':
                P_MAX = fn;
                motors_param_geted|=16;
                break;
            case 'v':
                V_MAX = fn;
                motors_param_geted|=32;
                break;
            case 'x':
                KP_MAX = fn;
                motors_param_geted|=64;
                break;
            case 'd':
                KD_MAX = fn;
                motors_param_geted|=128;
                break;
            case 'f':
                I_FW_MAX = fn;
                motors_param_geted|=256;
                break;
            case 'h':
                TEMP_MAX = fn;
                motors_param_geted|=512;
                break;
            case 'c':
                I_MAX_CONT = fn;
                motors_param_geted|=1024;
                break;
            case 'a':
                I_CAL = fn;
                motors_param_geted|=2048;
                break;
            case 'i':
                CAN_ID = int(fn);
                motors_param_geted|=4096;
                break;
            case 'm':
                CAN_MASTER = int(fn);
                motors_param_geted|=8192;
                break;
            case 't':
                CAN_TIMEOUT = int(fn);
                motors_param_geted|=16384;
                break;
            default:
                qDebug()<<"[unpack_config] unknow cmd: "<<data[2];
                return false;
        }
        return true;
    }
};

struct MotorData{
    MotorConfig config;

    double pos=0;
    double vel=0;
    double torque=0;

    uint64_t timestamp;

    MotorState state = MotorState::Dead;
 
    uint64_t encoder_msg_cnt = 0;
    bool wait_motor_mode=false;

    double encoder_anglemulti = 0;
    int encoder_linelization_count = 0;
};


class MyMotorTool : public QMainWindow
{
    Q_OBJECT

public:
    MyMotorTool(QWidget *parent = nullptr);
    ~MyMotorTool();
protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void time_out();
    void on_btn_refresh_clicked();
    void on_btn_connect_clicked();
    void on_btn_stop_clicked();
    void on_btn_clear_clicked();


    void on_cbx_select_motor_canid_currentIndexChanged(const QString& arg1 );
    void on_btn_select_clicked();
    void on_btn_calibration_encoder_clicked();
    void on_btn_set_zero_clicked();
    void on_btn_encoder_mode_clicked();
    void on_btn_setup_mode_clicked();
    void on_btn_motor_mode_clicked();

    void on_btn_exit_setup_mode_clicked();
    void on_btn_exit_motor_mode_clicked();
    void on_btn_exit_encoder_mode_clicked();
    void on_btn_copy_from_motor_clicked();
    void on_btn_write_config_clicked();
    void on_btn_show_plot_clicked();

    void on_spin_pos_valueChanged(double v);
    void on_spin_vel_valueChanged(double v);
    void on_spin_kp_valueChanged(double v);
    void on_spin_kd_valueChanged(double v);
    void on_spin_torque_valueChanged(double v);
    void on_slider_pos_valueChanged(int vv);
    void on_slider_vel_valueChanged(int vv);
    void on_slider_kp_valueChanged(int vv);
    void on_slider_kd_valueChanged(int vv);
    void on_slider_torque_valueChanged(int vv);


    void on_cbx_reply_content_currentIndexChanged(const QString& item_name);

private:
    const unsigned char cmd_motor[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc};
    const unsigned char cmd_zero[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe};
    const unsigned char cmd_setup[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb};
    const unsigned char cmd_cal[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfa};
    const unsigned char cmd_enc[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf9};
    const unsigned char cmd_esc[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfd};
    const unsigned char cmd_nothing[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x01};

    const unsigned char cmd_set_reply_pos_vel_torque[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8};
    const unsigned char cmd_set_reply_pos_timestamp[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7};

    const char setup_cmds[15] = {
        'g','k','b','l','p',
        'v','x','d','f','h',
        'c','a','i','m','t'
    };
private:
    Ui::MyMotorTool *ui;
    QList<QDoubleSpinBox*> double_spins;
    QList<QLabel*>  cfg_labels;
    bool is_exit = false;
    bool is_exit_recv = false;
    bool not_writed = true;
    bool want_send_motor_control_frame = false;
    bool wait_motor_mode=false;
    int desired_cmd_changed_flag;
    bool in_recv_cfg=false;
    bool only_try_get_params=false;

    void list_interfaces();

    int socket_=0;
    bool connected = false;

    QMap<QString,int> recv_motor_id_list;
    QMap<int,MotorData>     motors_data;
    QColor color[13];
    QElapsedTimer timer;

    QWidget* dialog_plot;
    int dialog_plot_winx = -1;
    int dialog_plot_winy = -1;
    int dialog_plot_winw = -1;
    int dialog_plot_winh = -1;
    QCustomPlot* qplot;
    bool start_plot=false;
    int last_plot_time=0;


    int this_can_id=-1;

    uint64_t timestamp = 0;

    std::thread can_recv_thread;

    void recv_msg();
    void unpack_msg(const can_frame& frame);

    void update_gui(int id);
    void update_setupmode_gui(int id);
    void update_motormode_gui(int id);
    void update_encodermode_gui(int id);


    void send_cmd(const unsigned char* data);

    void unpack_motor_msg(const can_frame& frame,float& cur_pos,float& cur_vel,float& cur_torque);
    void unpack_motor_msg(const can_frame& frame,float &cur_pos,uint64_t& timestamp);
    void unpack_motor_msg(const can_frame& frame,float &angle,int& encoder_count);

    void motormode_set_desired_pos(float pos);
    void motormode_set_desired_vel(float vel);
    void motormode_set_desired_torque(float torque);

    can_frame control_canframe;
    void make_contorl_canframe();
    bool valueChange(QSlider* slider,float v);
    bool valueChange(QSlider* slider,QDoubleSpinBox* spin);

    void make_zero_contorl_canframe();

    bool is_init();

    void create_plot_window();
protected:


};
#endif // MAINWINDOW_H
