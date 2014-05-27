/**********************************************************************
* @File    : 	Protocol.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.12.15
* @Brief   : 	数据传输协议
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
	UInt16 ResReact;	//阻抗
	UInt16 CapReact;	//容抗
}ReactParam;

// Define
#define PROTOCOL_RETRY_TIMES	0x02

// Public
void Protocol_AnalyzePacket(DataPak_t* pdataPacket);

// 主机->手持器
BOOL Protocol_CheckSlaveOnline();
BOOL Protocol_SetSlaveReady(BOOL* isSlaveReady);
BOOL Protocol_SetSlaveStart(BOOL* isSlaveRun);
BOOL Protocol_SetSlaveAbort(BOOL* isSlaveStoped);
WorkState Protocol_GetSlaveWorkState();
BOOL Protocol_SendReactParams(ReactParam* reactParam);


#endif
