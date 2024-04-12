

# 小米电机的一些相关软件 
包括：
1. my_motorcontrol 电机驱动板固件
2. MyMotorTool  电机上位机
3. cybermotor_ctrl 电机控制例子
4. socketcan-demo socketcan例子
5. stlink 编译好的烧写工具





## socketcan 用法
1. 插入USBCAN

2. 执行以下命令

sudo modprobe can
sudo modprobe vcan
sudo modprobe slcan

#列出SocketCan接口
ip link show

#显示can0信息
ifconfig can0 或 ip addr ls dev can0


#设置can0
sudo ip link set can0 type can bitrate 1000000

#启动can0
sudo ip link set up can0

#发送数据
cansend can0 001#FFFFFFFFFFFFFFFD 

#接收数据
candump can0





# 上位机操作流程
0. 进入MyMotorTool目录，build下的可执行文件为上位机。如果没有可编译获得，编译命令:cd build && cmake .. && make -j4
1. 插入支持socketcan的USBCAN 

2. 执行上述命令，开启can0 

3. USBCAN连接电机的can线 

4. 启动上位机，选择接口(一般是can0),点击连接 

5. 电机上电 

6. 观察上位机第2页CAN Receive，看是否收到数据 

7. 未收到数据，排查通讯故障

8. 收到数据，转到上位机第3页，此时应该能看的一个can id

9. 点击select，选择此id代表的电机

10. 选择后可在下方按钮进行操作

11. 进入设置模式(点击Setup Mode按钮)。等待界面出现。界面出现后，修改参数值，点击"Write Config To Motor"按钮则保存参数到电机

12. 点击"Exit Setup Mode"按钮，可退出设置模式 

13. 进入马达模式(点击Motor Mode按钮)。等待界面出现。界面出现后，修改位置增益KP(例如3),修改速度增益KD(例如1),此时可听见电机电流声。

14. 缓慢拖动位置(Pos)的滑块，进行位置控制，观察电机的转动。

15. 修改位置增益KP(例如0),修改速度增益KD(例如3),此时可听见电机电流声。缓慢拖动速度(Vel)的滑块，进行速度控制，观察电机的转动。

16. 点击"Exit Motor Mode"按钮，可退出马达模式，此时电流声消失

17. 点击 "Calibration Encoder"按钮，可进行编码器标定，此功能没有界面显示，标定完后目前没有提示

18. 点击 "Set Zero Position"按钮，可进行零位设置，此功能没有界面显示，设置完后没有提示，要观察设定结果，可进入马达模式，观察顶部的位置值是否接近0




# 可能的异常现象 
1. 点击在马达模式运动时不正常，抖动，有异响 
   排除硬件外的原因，可能的原因是: 电机未标定。
   验证方法：串口终端，进入编码器模式，观察编码器值，主要观察线性化修正哪里，如果修正量过大，则是编码器为标定
   故障复现：
             1.可能是新烧写的固件;(未标定，编码器校正表未初始化)
             2.以前烧写过的固件，而且标定过，现在重新烧写了版本不一样固件;(已标定，但不同版本固件的编码器校正表的保存地址不一样)
             3.。。。

    解决方法： 再标定一次
    
2. 先启动电机，后连接上位机，上位机收不到数据，没有出现Can_ID供选择
    可能的原因: 点击进入了设置模式或其他模式，导致电机不在主菜单，这时电机不会主动发送can消息，电机只有在主菜单才会主动发送CAN消息。
    解决方法1： 串口终端连接电机，并且回到主菜单。此时可看的上位机开始收can消息，并且能出现can id 
    解决方法2： 上位机点击第3页，在Custom下的输入框里输入电机的can id，点击select。此时可看到电机开始发消息，上位机"Can Receive"页出现消息数据
    解决方法3： 电机重新上电
    如果以上方法都收不到数据，排查can通信故障


# 串口CAN通信测试 
1. 串口终端连接电机，进入主菜单，按n，回车。此时看到串口在打印一些数据，这些数据是会同时通过CAN发送出去的 
2. 使用candump观察CAN消息，观察是否与串口终端中打印的一致
3. 注意，小米电机的can线，红色为CAN_L，黑色为CAN_H

# 串口编码器模式 
1. 串口终端连接电机，进入主菜单，按e，回车。此时看到串口在打印一些数据，这些数据是编码器的原始值，线性化校正值，单圈值，多圈值，以及圈数等
2. 注意观察 线性化校正值 与 原始值的 差异，如果差异较大，可能编码器未标定。比如，新刚烧写固件时，某些校正量可能是NaN，可能是几十亿等超大的数字。正常差异值是在几十的比较多，少数超过100的



# 电机控制命令
1. 打开MyMotorTool项目的MyMotorTool.h文件，可以看到定义的几条指令：
    进入马达模式：
    const unsigned char cmd_motor[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc};
    设置零位:
    const unsigned char cmd_zero[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe};
    进入设置模式:
    const unsigned char cmd_setup[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb};
    进入标定模式:
    const unsigned char cmd_cal[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfa};
    进入编码器模式(因功能不太实用,目前未开发):
    const unsigned char cmd_enc[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf9};
    退出当前状态，进入主菜单:
    const unsigned char cmd_esc[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfd};
    空命令，电机收到此消息不进行任何操作。但会回复一帧CAN消息
    const unsigned char cmd_nothing[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x01};

2. 如果要控制电机，可参考MyMotorTool.cpp中make_control_frame函数，此函数生成一个电机控制命令。使用的协议与mit一致
    例子代码如下
    //将kp，kd，torque，pos，vel等值，使用float_to_uint函数，转换为一个整型的值。
    int p_int = float_to_uint(ui->spin_pos->value(), P_MIN, P_MAX, 16);
    int v_int = float_to_uint(ui->spin_vel->value(), V_MIN, V_MAX, 12);
    int kp_int = float_to_uint(ui->spin_kp->value(), KP_MIN, KP_MAX, 12);
    int kd_int = float_to_uint(ui->spin_kd->value(), KD_MIN, KD_MAX, 12);
    int tq_int = float_to_uint(ui->spin_torque->value(), -I_MAX*KT*GR, I_MAX*KT*GR, 12);
    
    //将转换好的整型的值打包进CAN的Frame中
    unsigned char* data = control_canframe.data;
    data[0] = p_int >> 8;
    data[1] = p_int & 0xff;
    data[2] = v_int >> 4;
    data[3] = ((v_int & 0xf)<<4) | (kp_int>>8);
    data[4] = kp_int & 0xff;
    data[5] = kd_int >> 4;
    data[6] = ((kd_int & 0xf)<<4) | (tq_int>>8);
    data[7] = tq_int & 0xff;

3. 其中，can数据定义如下
        /// CAN Command Packet Structure ///
        /// 16 bit position command, between P_MIN and P_MAX                    (rad)
        /// 12 bit velocity command, between V_MIN and V_MAX                    (rad/s)
        /// 12 bit kp, between KP_MIN and KP_MAX                                (N-m/rad)
        /// 12 bit kd, between KD_MIN and 100 KD_MAX                            (N-m*s/rad)
        /// 12 bit feed forward torque, between -I_MAX*KT*GR and I_MAX*KT*GR    (N-m)
        /// CAN Packet is 8 8-bit words
        /// Formatted as follows.  For each quantity, bit 0 is LSB
        /// 0: [position[15-8]]
        /// 1: [position[7-0]]
        /// 2: [velocity[11-4]]
        /// 3: [velocity[3-0], kp[11-8]]
        /// 4: [kp[7-0]]
        /// 5: [kd[11-4]]
        /// 6: [kd[3-0], torque[11-8]]
        /// 7: [torque[7-0]]

4. 电机收到can消息后，必定回复的消息如下
        /// CAN Reply Packet Structure ///
        /// 16 bit position, between between P_MIN and P_MAX            (rad)
        /// 12 bit velocity, between V_MIN and V_MAX                    (rad/s)
        /// 12 bit current, between -I_MAX and I_MAX                    (A)
        /// CAN Packet is 5 8-bit words
        /// Formatted as follows.  For each quantity, bit 0 is LSB
        /// 0: [can_id]
        /// 1: [position[15-8]]
        /// 2: [position[7-0]]
        /// 3: [velocity[11-4]]
        /// 4: [velocity[3-0], current[11-8]]
        /// 5: [current[7-0]]
        /// 6: [0xfe]
        /// 7: [0xef]

5. 注意事项: 
    根据以上3和4的协议，可知一个数字通过CAN发送到电机，电机再解析出来，需要一个浮点转整数(函数float_to_uint)，并在电机端进行整数转浮点数(函数float_to_uint的反函数)的过程
    可知，必须要通信的双方使用一致的参数(例如上述的KP_MAX等)才能正确解析！否则电机的行为可能无法预测！！！








VM 欠压闭锁 (UVLO)
电荷泵欠压 (CPUV)
MOSFET 过流保护 (OCP)
栅极驱动器故障 (GDF)
热警告和热关断 (OTW/OTSD)
故障状态指示器 (nFAULT)
