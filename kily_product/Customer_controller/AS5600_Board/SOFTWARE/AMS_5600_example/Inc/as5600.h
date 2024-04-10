/*
   ���������Ա������������ṩ
	 AS5600Ӳ���������ӣ�https://item.taobao.com/item.htm?spm=a1z38n.10677092.0.0.2c351debwMQV2w&id=584601996739
*/
/*******************************************************************************
* �ļ����ƣ�as5600.h
*
* ժ    Ҫ��1.�������ģ��I2Cͨ��Э��
*           2.��ʼ��as5600���������,
*
* ��ǰ�汾��
* ��    �ߣ���������	
* ��    �ڣ�2018/12/31
* ���뻷����keil5
*
* ��ʷ��Ϣ��
*******************************************************************************/

#ifndef __MD_AS5600_H
#define __MD_AS5600_H

#include "stm32f0xx_hal.h"  //�����Ҫ���ݲ�ͬ��оƬ���ͽ��б任
#include "main.h"

#define SDA1_IN()  IIC_SDA_GPIO_Port->MODER &= ~(0x3<<(10*2))
#define SDA1_OUT() IIC_SDA_GPIO_Port->MODER &= ~(0x3<<(10*2));IIC_SDA_GPIO_Port->MODER |= (0x1<<(10*2))

#define Sim_I2C1_SCL_LOW          (IIC_SCL_GPIO_Port->ODR &= ~(IIC_SCL_Pin))
#define Sim_I2C1_SCL_HIG          (IIC_SCL_GPIO_Port->ODR |=  (IIC_SCL_Pin))
#define Sim_I2C1_SDA_LOW          (IIC_SDA_GPIO_Port->ODR &= ~(IIC_SDA_Pin))
#define Sim_I2C1_SDA_HIG          (IIC_SDA_GPIO_Port->ODR |=  (IIC_SDA_Pin))

#define Sim_I2C1_SDA_STATE        (IIC_SDA_GPIO_Port->IDR &= (IIC_SDA_Pin))

#define Sim_I2C1_DELAY 		Sim_I2C1_Delay(100000)
#define Sim_I2C1_NOP		  Sim_I2C1_Delay(400)  //25 

#define Sim_I2C1_READY			0x00
#define Sim_I2C1_BUS_BUSY		0x01
#define Sim_I2C1_BUS_ERROR	0x02

#define Sim_I2C1_NACK	      0x00
#define Sim_I2C1_ACK				0x01

#define I2C1_Direction_Transmitter 		0x00
#define I2C1_Direction_Receiver   	  0x01

#define AS5600_ADDRESS_MAG           0x36

typedef enum
{
/* set i2c address */ 
  _ams5600_Address = 0x36,
  _zmco = 0x00,
  _zpos_hi = 0x01,
  _zpos_lo = 0x02,
  _mpos_hi = 0x03,
  _mpos_lo = 0x04,
  _mang_hi = 0x05,
  _mang_lo = 0x06,
  _conf_hi = 0x07,    
  _conf_lo = 0x08,
  _raw_ang_hi = 0x0c,
  _raw_ang_lo = 0x0d,
  _ang_hi = 0x0e,
  _ang_lo = 0x0f,
  _stat = 0x0b,
  _agc = 0x1a,
  _mag_hi = 0x1b,
  _mag_lo = 0x1c,
  _burn = 0xff
}AMS5600Registers_t;


void PrintMenu(void);
void Programe_Run(void);
#endif 
		
