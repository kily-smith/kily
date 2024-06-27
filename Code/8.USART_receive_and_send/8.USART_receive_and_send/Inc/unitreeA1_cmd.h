
#ifndef __UNITREEA1_CMD__
#define __UNITREEA1_CMD__

#include "motor_msg.h"
#include "usart.h"

extern motor_send_t cmd_left;  // ����һ�ŵ��������
extern motor_send_t cmd_right; // ����һ�ŵ��������

extern motor_recv_t Date_left;        // ���ȵ������������
extern motor_recv_t id00_left_date;   // ����00�ŵ������������
extern motor_recv_t id01_left_date;   // ����01�ŵ������������
extern motor_recv_t id02_left_date;   // ����02�ŵ������������

extern motor_recv_t Date_right;       // ���ȵ������������
extern motor_recv_t id00_right_date;  // ����00�ŵ������������
extern motor_recv_t id01_right_date;  // ����01�ŵ������������
extern motor_recv_t id02_right_date;  // ����02�ŵ������������

/**
 @brief ��Ӧ��������޸�
 @param send Ϊcmd_left��cmd_right���ֱ�������Ҳ��Ȳ�
 @param id   ���ͽ���Ŀ������id
 @param pos  Ϊ�����תȦ����1ΪһȦ
 @param KP   ����ն�ϵ��
 @param KW   ����ٶ�ϵ��
*/
void modfiy_cmd(motor_send_t *send,uint8_t id, float Pos, float KP, float KW);

// �ٶ�ģʽ
void modfiy_speed_cmd(motor_send_t *send,uint8_t id, float Omega);

// ����ģʽ
void modfiy_torque_cmd(motor_send_t *send,uint8_t id, float torque);


/// @brief �����͵��ͨѶ�Ĵ��룬����ȡ�����ݴ����Ӧ�ṹ����
/// @param huart ��Ҫʹ�õĴ��ڣ�huart1Ϊ��࣬6Ϊ�Ҳ�

void unitreeA1_rxtx(UART_HandleTypeDef *huart);

uint32_t crc32_core(uint32_t *ptr, uint32_t len);

#endif
