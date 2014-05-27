/**********************************************************************
* @File    : 	Protocol.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.12.15
* @Brief   : 	���ݴ���Э��
***********************************************************************/
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

// Include
#include "DataPacket.h"

// Enum
typedef enum WorkState_t {
    WorkState_UnReady = 0,
    WorkState_Ready,
    WorkState_Running,
    WorkState_Stopped,
    WorkState_Error,
    WorkState_UnKnow
}WorkState;

// Struct
typedef struct ReactParam_t {
	UInt16 ResReact;	//�迹
	UInt16 CapReact;	//�ݿ�
}ReactParam;

// Define
#define PROTOCOL_RETRY_TIMES	0x02

// Public
void Protocol_AnalyzePacket(DataPak_t* pdataPacket);

// ����->�ֳ���
BOOL Protocol_CheckSlaveOnline();
BOOL Protocol_SetSlaveReady(BOOL* isSlaveReady);
BOOL Protocol_SetSlaveStart(BOOL* isSlaveRun);
BOOL Protocol_SetSlaveAbort(BOOL* isSlaveStoped);
WorkState Protocol_GetSlaveWorkState();
BOOL Protocol_SendReactParams(ReactParam* reactParam);


#endif
