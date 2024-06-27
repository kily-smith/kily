#ifndef MOTOR_MSG
#define MOTOR_MSG

#include <stdint.h>
#include "motor_msg.h"

#pragma pack(1)

// �����õ����������ݽṹ
typedef union
{
    int32_t L;
    uint8_t u8[4];
    uint16_t u16[2];
    uint32_t u32;
    float F;
	
} COMData32;

typedef struct
{
    // ���� ���ݰ�ͷ
    uint8_t start[2]; // ��ͷ
    uint8_t motorID;  // ���ID  0,1,2,3 ...   0xBB ��ʾ�����е���㲥����ʱ�޷��أ�
    uint8_t reserved;
	
} COMHead;

#pragma pack()

#pragma pack(1)


typedef struct
{                      // �� 4���ֽ�һ������ ����Ȼ�����������
                       // ���� ����
    uint8_t mode;      // �ؽ�ģʽѡ��
    uint8_t ModifyBit; // ������Ʋ����޸�λ
    uint8_t ReadBit;   // ������Ʋ�������λ
    uint8_t reserved;

    COMData32 Modify; // ��������޸� ������
	
    //ʵ�ʸ�FOC��ָ������Ϊ��
    // K_P*delta_Pos + K_W*delta_W + T
    int16_t T;     // �����ؽڵ�������أ������������أ�x256, 7 + 8 ����
    int16_t W;     // �����ؽ��ٶ� �����������ٶȣ� x128,       8 + 7����
    int32_t Pos; // �����ؽ�λ�� x 16384/6.2832, 14λ������������0������������ؽڻ����Ա�����0��Ϊ׼��

    int16_t K_P; // �ؽڸն�ϵ�� x2048  4+11 ����
    int16_t K_W; // �ؽ��ٶ�ϵ�� x1024  5+10 ����

    uint8_t LowHzMotorCmdIndex; // �����Ƶ�ʿ������������, 0-7, �ֱ����LowHzMotorCmd�е�8���ֽ�
    uint8_t LowHzMotorCmdByte;  // �����Ƶ�ʿ���������ֽ�

    COMData32 Res[1]; // ͨѶ �����ֽ�  ����ʵ�ֱ��һЩͨѶ����

} MasterComdV3; // �������ݰ��İ�ͷ ��CRC 34�ֽ�


/*���ͱ����趨*/
typedef struct
{
    // ���� ��������������ݰ�
    COMHead head;
    MasterComdV3 Mdata;
    COMData32 CRCdata;
	
} MasterComdDataV3; //��������

//#pragma pack()
//#pragma pack(1)

#pragma pack()

#pragma pack(1)


/*�������ݰ�����*/
typedef struct
{ // �� 4���ֽ�һ������ ����Ȼ�����������
    // ���� ����
    uint8_t mode;    // ��ǰ�ؽ�ģʽ
    uint8_t ReadBit; // ������Ʋ����޸�     �Ƿ�ɹ�λ
    int8_t Temp;     // �����ǰƽ���¶�
    uint8_t MError;  // ������� ��ʶ

    COMData32 Read; // ��ȡ�ĵ�ǰ ��� �Ŀ�������
    int16_t T;      // ��ǰʵ�ʵ���������       7 + 8 ����

    int16_t W; // ��ǰʵ�ʵ���ٶȣ����٣�   8 + 7 ����
    float LW;  // ��ǰʵ�ʵ���ٶȣ����٣�

    int16_t W2; // ��ǰʵ�ʹؽ��ٶȣ����٣�   8 + 7 ����
    float LW2;  // ��ǰʵ�ʹؽ��ٶȣ����٣�

    int16_t Acc;    // ���ת�Ӽ��ٶ�       15+0 ����  ������С
    int16_t OutAcc; // �������ٶ�         12+3 ����  �����ϴ�

    int32_t Pos;  // ��ǰ���λ�ã�����0������������ؽڻ����Ա�����0��Ϊ׼��
    int32_t Pos2; // �ؽڱ�����λ��(���������)

    int16_t gyro[3]; // ���������6�ᴫ��������
    int16_t acc[3];

    // ��������������
    int16_t Fgyro[3]; //
    int16_t Facc[3];
    int16_t Fmag[3];
    uint8_t Ftemp; // 8λ��ʾ���¶�  7λ��-28~100�ȣ�  1λ0.5�ȷֱ���

    int16_t Force16; // ����������16λ����
    int8_t  Force8;   // ����������8λ����

    uint8_t FError; //  ��˴����������ʶ

    int8_t Res[1]; // ͨѶ �����ֽ�

} ServoComdV3; // �������ݰ��İ�ͷ ��CRC 78�ֽڣ�4+70+4��



//�ܵĵ�����ձ���//
typedef struct
{
    // ���� ��������������ݰ�
    COMHead head;
	
    ServoComdV3 Mdata;

    COMData32 CRCdata;

} ServoComdDataV3; //��������



//�ܵĵ����������//
typedef struct
{
        // ���� ���͸�ʽ������
		MasterComdDataV3 motor_send_data;
	
        int hex_len;   
        unsigned short id;   //���ID��0xBB����ȫ�����
        unsigned short mode; // 0:����, 5:����ת��, 10:�ջ�FOC����
        float T;             //�����ؽڵ�������أ������������أ���Nm��
        float W;             //�����ؽ��ٶȣ����������ٶȣ�(rad/s)
        float Pos;           //�����ؽ�λ�ã�rad��
        float K_P;           //�ؽڸն�ϵ��
        float K_W;           //�ؽ��ٶ�ϵ��

        COMData32 Res;                    // ͨѶ �����ֽ�  ����ʵ�ֱ��һЩͨѶ����
      //����������ݽṹ�壬���motor_msg.h
}motor_send_t;



//�ܵĵ����������//
typedef struct
{
        // ���� ��������

        ServoComdDataV3 motor_recv_data; //����������ݽṹ�壬���motor_msg_A1B1.h

        int hex_len;       //���յ�16�����������鳤��, 78

		//bool correct;   //���������Ƿ�������true������false��������
		//uint8_t right_Date[MOTOR_RX_LENGTH];
	 
	    unsigned char motor_id; // 1Byte ���ID
        unsigned char mode;     // 1Byte 0:����, 5:����ת��, 10:�ջ�FOC����
        int Temp;               // 1Byte �¶�
        int MError;             // 1Byte ������

        float T;                // 2Byte ��ǰʵ�ʵ���������
        float W;                // 2Byte ��ǰʵ�ʵ���ٶȣ����٣�
        float Pos;              // 4Byte ��ǰ���λ��
        float LW;               // 4Byte ��ǰʵ�ʵ���ٶȣ����٣�
        int Acc;                // 2Byte ���ת�Ӽ��ٶ�

        float Pos2;             // ��Ӳ���
	
        float gyro[3]; // ���������6�ᴫ��������
        float acc[3];
		
} motor_recv_t; // ���� Date_left Date_right


typedef struct
{
	motor_send_t motor_send;
	motor_recv_t motor_recv;
	
}unitree_motor_t;


typedef struct
{
	        unsigned char mode;     // 0:����, 5:����ת��, 10:�ջ�FOC����

	        float T;                // ����������
			float W;                // ����ٶ�
			float Pos;              // ���λ��
	        float K_P;              //�ؽڸն�ϵ��
	        float K_W;              //�ؽ��ٶ�ϵ��
	
}motor_control_t;


typedef struct
{
    motor_control_t motor_control;
    motor_recv_t motor_recv;
    motor_send_t motor_send;

}motorcmd;



// typedef struct {
// ���� �ܵ�485 �������ݰ�

//   ServoComdDataV3 M[3];
//  // uint8_t  nullbyte1;

// }DMA485RxDataV3;

#pragma pack()


#endif
