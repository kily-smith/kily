#ifndef CONTROLCAN_H
#define CONTROLCAN_H

#include <string>
#include <stdio.h>
#include <Windows.h>
#include <QLibrary>

//接口卡类型定义
#define LCAN_USBCAN1		        3
#define LCAN_USBCAN2		        4
#define LCAN_USBCANFDMini           43
#define LCAN_USBCANFD1              42
#define LCAN_USBCANFD2              41

#define CAN_DATA_DLC_MAX            8
#define CANFD_DATA_DLC_MAX          64

//函数调用返回状态值
#define	LCAN_STATUS_OK			    1
#define LCAN_STATUS_ERR			    0

//FD FLAG
#define LCAN_CANFD_BRS              0x08
#define LCAN_CANFD_ESI              0x10
//FD FLITER
#define LCAN_FLITER_CMD_CLR         0x00
#define LCAN_FLITER_CMD_ADD         0x01
#define LCAN_FLITER_CMD_ENABLE      0x02


/////////////////////////////////////////////////////////////////////////////
//1.USBCAN系列设备信息的数据类型。
typedef  struct  _LCAN_BOARD_INFO{
    USHORT	hw_Version;//硬件版本
    USHORT	fw_Version;//固件版本
    USHORT	dr_Version;//驱动程序版本
    USHORT	in_Version;//接口库版本
    USHORT	irq_Num;   //板卡所使用的中断号
    BYTE	can_Num;   //CAN通道数
    CHAR	serialNumber[20];//序列号
    CHAR	str_hw_Type[40]; //硬件型号
    USHORT	reserved[4];
} LCAN_BOARD_INFO,*PLCAN_BOARD_INFO;

//CAN信息帧的数据类型。
typedef  struct  _LCAN_CAN_OBJ{
    UINT	id;
    UINT	timeStamp;
    UCHAR	timeFlag;
    UCHAR	sendType;
    UCHAR	remoteFlag;//是否是远程帧
    UCHAR	externFlag;//是否是扩展帧
    UCHAR	dataLen;
    UCHAR	data[CAN_DATA_DLC_MAX];
    UCHAR	reserved[3];
}LCAN_CAN_OBJ,*PLCAN_CAN_OBJ;

//CANFD信息帧的数据类型。
typedef  struct  _LCAN_CANFD_OBJ{
    UINT	id;
    UINT	timeStamp;
    UCHAR	flag;//CANFD_BRS/ESI
    UCHAR	sendType;
    UCHAR	remoteFlag;//是否是远程帧
    UCHAR	externFlag;//是否是扩展帧
    UCHAR	dataLen;
    UCHAR	data[CANFD_DATA_DLC_MAX];
    UCHAR	reserved[3];
}LCAN_CANFD_OBJ,*PLCAN_CANFD_OBJ;


//定义CAN的配置信息 用于CAN设备
typedef struct _LCAN_INIT_CONFIG{
    UINT	accCode;
    UINT	accMask;
    UINT	reserved;
    UCHAR	filter;
    UCHAR	timing0;
    UCHAR	timing1;
    UCHAR	mode;
}LCAN_INIT_CONFIG,*PLCAN_INIT_CONFIG;

//定义CANFD的配置信息 用于FD设备
typedef struct _LCAN_INITFD_CONFIG{
    UINT	abitBaudHz;
    UINT	dbitBaudHz;
    UINT	abit_timing;
    UINT	dbit_timing;
    UCHAR	mode;
    UCHAR	fdEn;
    UCHAR	isoEn;
    UCHAR	rev2;
}LCAN_INITFD_CONFIG,*PLCAN_INITFD_CONFIG;

//定义过滤器配置信息
typedef struct _LCAN_FLITER_CONFIG{
    UCHAR	cmd;
    UCHAR	fliterMode;
    UCHAR	idType;
    UCHAR	rev1;
    UINT	fliterStardId;
    UINT	fliterEndId;
}LCAN_FLITER_CONFIG,*PLCAN_FLITER_CONFIG;

typedef DWORD (WINAPI *LCAN_OpenDevice)(UINT devType, UINT devIndex);
typedef DWORD (WINAPI *LCAN_CloseDevice)(UINT devType, UINT devIndex);
typedef DWORD (WINAPI *LCAN_GetReceiveNum)(UINT devType,UINT devIndex,UINT canIndex,UCHAR type);
typedef DWORD (WINAPI *LCAN_Receive)(UINT devType,UINT devIndex,UINT canIndex,PLCAN_CAN_OBJ pReceive,UINT len,INT waitTime);
typedef DWORD (WINAPI *LCAN_ReceiveFD)(UINT devType,UINT devIndex,UINT canIndex,PLCAN_CANFD_OBJ pReceive,UINT len,INT waitTime);
typedef DWORD (WINAPI *LCAN_Transmit)(UINT devType,UINT devIndex,UINT canIndex,PLCAN_CAN_OBJ pSend,UINT len);
typedef DWORD (WINAPI *LCAN_TransmitFD)(UINT devType,UINT devIndex,UINT canIndex,PLCAN_CANFD_OBJ pSend,UINT len);
typedef DWORD (WINAPI *LCAN_InitCAN)(UINT devType, UINT devIndex, UINT canIndex, PLCAN_INIT_CONFIG pInitConfig);
typedef DWORD (WINAPI *LCAN_InitCANFD)(UINT devType, UINT devIndex, UINT canIndex, PLCAN_INITFD_CONFIG pInitConfig);
typedef DWORD (WINAPI *LCAN_SetFliter)(UINT devType, UINT devIndex, UINT canIndex, PLCAN_FLITER_CONFIG pFliterConfig);
#endif
