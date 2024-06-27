#include "main.h"
#include "motor_msg.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "unitreeA1_cmd.h"
#include "main.h"

#define PI 3.14159

motor_send_t cmd_left;        // ����һ�ŵ��������
motor_send_t cmd_right;       // ����һ�ŵ��������

motor_recv_t Date_left;       // ���ȵ������������
motor_recv_t id00_left_date;  // ����00�ŵ������������
motor_recv_t id01_left_date;  // ����01�ŵ������������
motor_recv_t id02_left_date;  // ����02�ŵ������������

motor_recv_t Date_right;      // ���ȵ������������
motor_recv_t id00_right_date; // ����00�ŵ������������
motor_recv_t id01_right_date; // ����01�ŵ������������
motor_recv_t id02_right_date; // ����02�ŵ������������

// CRCУ��λ�Ĵ���
uint32_t crc32_core_Ver3(uint32_t *ptr, uint32_t len)
{
    uint32_t bits;
    uint32_t i;
    uint32_t xbit = 0;
    uint32_t data = 0;
    uint32_t CRC32 = 0xFFFFFFFF;
    const uint32_t dwPolynomial = 0x04c11db7;
    for (i = 0; i < len; i++)
    {
        xbit = 1 << 31;
        data = ptr[i];
        for (bits = 0; bits < 32; bits++)
        {
            if (CRC32 & 0x80000000)
            {
                CRC32 <<= 1;
                CRC32 ^= dwPolynomial;
            }
            else
                CRC32 <<= 1;
            if (data & xbit)
                CRC32 ^= dwPolynomial;

            xbit >>= 1;
        }
    }
    return CRC32;
}

// ���λ���޸�
void modfiy_cmd(motor_send_t *send,uint8_t id, float Pos, float KP, float KW)
{

    send->hex_len = 34;

    send->mode = 10;
	send->id   = id;

    send->Pos  = 2*PI/360*9.1*Pos;  // 6.2832 = 2 PI // ԭ��Ϊ 6.2832*9.1*2*Pos
    send->W    = 0;
    send->T    = 0.0;
    send->K_P  = KP;
    send->K_W  = KW;
}

// ����ٶ��޸�
void modfiy_speed_cmd(motor_send_t *send,uint8_t id, float Omega)
{

    send->hex_len = 34;

    send->mode = 10;
	send->id   = id;

    send->Pos  = 0;
    send->W    = Omega;
    send->T    = 0.0;
    send->K_P  = 0.0;
    send->K_W  = 3.0;
}

// ��������޸�
void modfiy_torque_cmd(motor_send_t *send,uint8_t id, float torque)
{

    send->hex_len = 34;

    send->mode = 10;
	send->id   = id;

    send->Pos  = 0.0;
    send->W    = 0.0;
    send->T    = torque;
    send->K_P  = 0.0;
    send->K_W  = 0.0;
}

// ������ͽ��պ���
void unitreeA1_rxtx(UART_HandleTypeDef *huart)
{

    /*�������������������������������������������������������������������������������������ȴ��뷶Χ������������������������������������������������������������������������������������������������*/
    if (huart == &huart1)
    {

        uint8_t A1cmd_left[34]; // ��������
        uint8_t Date[78];       // ��������

        // �˴�Ϊ���ȵ���ṹ��//
        cmd_left.motor_send_data.head.start[0] = 0xFE;
        cmd_left.motor_send_data.head.start[1] = 0xEE;
        cmd_left.motor_send_data.head.motorID  = cmd_left.id;
        cmd_left.motor_send_data.head.reserved = 0x00;

        cmd_left.motor_send_data.Mdata.mode      = cmd_left.mode;  // mode = 10
        cmd_left.motor_send_data.Mdata.ModifyBit = 0xFF;
        cmd_left.motor_send_data.Mdata.ReadBit   = 0x00;
        cmd_left.motor_send_data.Mdata.reserved  = 0x00;
        cmd_left.motor_send_data.Mdata.Modify.F  = 0;
        cmd_left.motor_send_data.Mdata.T         = cmd_left.T * 256;
        cmd_left.motor_send_data.Mdata.W         = cmd_left.W * 128;
        cmd_left.motor_send_data.Mdata.Pos       = (int)((cmd_left.Pos / 6.2832f) * 16384.0f); // ��λ rad
        cmd_left.motor_send_data.Mdata.K_P       = cmd_left.K_P * 2048;
        cmd_left.motor_send_data.Mdata.K_W       = cmd_left.K_W * 1024;

        cmd_left.motor_send_data.Mdata.LowHzMotorCmdIndex = 0;
        cmd_left.motor_send_data.Mdata.LowHzMotorCmdByte  = 0;
        cmd_left.motor_send_data.Mdata.Res[0] = cmd_left.Res;

        cmd_left.motor_send_data.CRCdata.u32 = crc32_core_Ver3((uint32_t *)(&cmd_left.motor_send_data), 7); // CRCУ��


        memcpy(A1cmd_left, &cmd_left.motor_send_data, 34);

        // HAL�� DMA �������� + ��������
        HAL_UART_Transmit_DMA(&huart1, A1cmd_left, 34);

        HAL_Delay(10);
        HAL_UART_Receive_DMA(&huart1, Date, 78);

        // �������ݴ���
        // 1.û�д����¶����� (������ȷ����Ϊ������?)
        // 2.������������Ƿ���ȷ
        Date_left.motor_recv_data.head.motorID = Date[2];  
        Date_left.motor_recv_data.Mdata.mode   = Date[4];  
        Date_left.motor_recv_data.Mdata.Temp   = Date[6];
        Date_left.motor_recv_data.Mdata.MError = Date[7]; 
        Date_left.motor_recv_data.Mdata.T      = Date[13] << 8  | Date[12]; // ��ƴ
        Date_left.motor_recv_data.Mdata.W      = Date[15] << 8  | Date[14]; // ��ƴ
        Date_left.motor_recv_data.Mdata.Acc    = Date[27] << 8  | Date[26]; // ��ƴ
        Date_left.motor_recv_data.Mdata.Pos    = Date[33] << 24 | Date[32] << 16 | Date[31] << 8 | Date[30];  // ��ƴ

        Date_left.motor_id = Date_left.motor_recv_data.head.motorID;                               // ID     ��ȷ
        Date_left.mode     = Date_left.motor_recv_data.Mdata.mode;                                 // mode   ��ȷ
        Date_left.Temp     = Date_left.motor_recv_data.Mdata.Temp;                                 // Temp   ��ȷ (����)
        Date_left.MError   = Date_left.motor_recv_data.Mdata.MError;                               // MError ��ȷ
        Date_left.T        = (float) Date_left.motor_recv_data.Mdata.T / 256;                      // T      ��ȷ
        Date_left.Pos      = (float) (Date_left.motor_recv_data.Mdata.Pos / (16384.0f/2/PI));      // Pos    ��ȷ
        Date_left.W        = (float) Date_left.motor_recv_data.Mdata.W / 128;                      // W      ��ȷ (С��)
        Date_left.Acc      = (float) Date_left.motor_recv_data.Mdata.Acc;                          // Acc    ò����ȷ (��ҪVOFA��ӡ���Կ��Ƿ�����)
        if(Date_left.Temp==0)
        {
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
                    HAL_Delay(1000);
        }
        if (Date_left.motor_id == 0x00)
        {
            id00_left_date.motor_id = Date_left.motor_id;
            id00_left_date.mode     = Date_left.mode; 
            id00_left_date.Temp     = Date_left.Temp;
            id00_left_date.MError   = Date_left.MError;
            id00_left_date.T        = Date_left.T;
            id00_left_date.W        = Date_left.W;   
            id00_left_date.Pos      = Date_left.Pos;
            id00_left_date.Acc      = Date_left.Acc; 
        }

        if (Date_left.motor_id == 0x01)
        {
            id01_left_date.motor_id = Date_left.motor_id;
            id01_left_date.mode     = Date_left.mode; 
            id01_left_date.Temp     = Date_left.Temp;
            id01_left_date.MError   = Date_left.MError;
            id01_left_date.T        = Date_left.T;
            id01_left_date.W        = Date_left.W;   
            id01_left_date.Pos      = Date_left.Pos;
            id01_left_date.Acc      = Date_left.Acc; 

        }

        if (Date_left.motor_id == 0x02)
        {
            id02_left_date.motor_id = Date_left.motor_id;
            id02_left_date.mode     = Date_left.mode; 
            id02_left_date.Temp     = Date_left.Temp;
            id02_left_date.MError   = Date_left.MError;
            id02_left_date.T        = Date_left.T;
            id02_left_date.W        = Date_left.W;  
            id02_left_date.Pos      = Date_left.Pos;
            id02_left_date.Acc      = Date_left.Acc; 
        }
    }

}

// ���0λ����
