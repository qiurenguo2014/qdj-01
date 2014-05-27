/**********************************************************************
* @File    : 	Protocol.cpp
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.12.15
* @Brief   : 	数据传输协议
***********************************************************************/
#include "rf.h"
#include "Protocol.h"


//================================
// 全局
//================================
typedef struct CommunStatus_t {
	BOOL IsOnline;
	WorkState DestState;
}CommunStatus;

// 接收标识符
typedef enum RecvFlag_t {
    RecvFlag_Null = 0,
    RecvFlag_Ack,
    RecvFlag_WorkReady,
    RecvFlag_WorkStart,
    RecvFlag_WorkAbort,
    RecvFlag_WorkState,
    RecvFlag_ReactParam,
    RecvFlag_CalibReq,
    RecvFlag_CalibAbort,
}RecvFlag;

static UInt8 destAddr = DPAK_ADDR_DEST;
static CommunStatus commStatus = {FALSE, WorkState_UnKnow};
static volatile RecvFlag recvFlag = RecvFlag_Null;


//================================
// 数据包解析
//================================
void Protocol_AnalyzePacket(DataPak_t* pdataPacket)
{
	UInt8 isSuccess = 0;
	//System_CommTicksReset();
	destAddr = pdataPacket->SourceAddr;

	switch(pdataPacket->Command) {
		// Ping
	case DPAK_CMD_SYS_ACK:
		recvFlag = RecvFlag_Ack;
		return;

		// 控制
	case DPAK_CMD_WORK_READY_CONFIRM:
		commStatus.DestState = (WorkState)(pdataPacket->BinData[0]);
		recvFlag = RecvFlag_WorkReady;
		return;

	case DPAK_CMD_WORK_START_CONFIRM:
		commStatus.DestState = (WorkState)(pdataPacket->BinData[0] + 1);
		recvFlag = RecvFlag_WorkStart;
		return;

	case DPAK_CMD_WORK_ABORT_CONFIRM:
		commStatus.DestState = (WorkState)(pdataPacket->BinData[0] + 2);
		recvFlag = RecvFlag_WorkAbort;
		return;

	case DPAK_CMD_WORK_RETSTATUS:
		commStatus.DestState = (WorkState)pdataPacket->BinData[0];
		recvFlag = RecvFlag_WorkState;
		return;

	case DPAK_CMD_WORK_ERROR:
		break;

		// 数据
	case DPAK_CMD_DAT_RETREACT:
		recvFlag = RecvFlag_ReactParam;
		return;

		// 校准
	case DPAK_CMD_CALIB_REQ:
		//【在此开启自校信号】
		UART_SendDataPacket(destAddr, DPAK_CMD_CALIB_CONFIRM, NULL, 0);
		return;

	case DPAK_CMD_CALIB_ABORT:
		//【在此关闭自校信号】
		UART_SendDataPacket(destAddr, DPAK_CMD_CALIB_ABORT_CONFIRM, NULL, 0);
		return;

	default:
		break;
	}
}

//================================
// 发送数据包
//================================
static BOOL sProtocol_WaitForSlaveAck(RecvFlag packetFlag)
{
	Int32 waitTime = 14;
	recvFlag = RecvFlag_Null;

	do {
		if(recvFlag == packetFlag)
			return TRUE;
		STK_delay10ms(1);
		//DelayMs(2);
	} while(waitTime--);

	return FALSE;
}

static BOOL sProtocol_SendNormalPacket(UInt8 command, RecvFlag packetFlag)
{
	Int32 tryTime = PROTOCOL_RETRY_TIMES;

	while(tryTime--) {
		UART_SendDataPacket(destAddr, command, NULL, 0);

		if(sProtocol_WaitForSlaveAck(packetFlag))
			return TRUE;
	}

	return FALSE;
}

//================================
// Master -> Slaver
//================================
// 通讯测试
BOOL Protocol_CheckSlaveOnline()
{
	return sProtocol_SendNormalPacket(DPAK_CMD_SYS_PING, RecvFlag_Ack);
}
// 准备指令
BOOL Protocol_SetSlaveReady(BOOL* isSlaveReady)
{
	if(sProtocol_SendNormalPacket(DPAK_CMD_WORK_READY, RecvFlag_WorkReady)) {
		*isSlaveReady = (BOOL)(commStatus.DestState == WorkState_Ready);
		return TRUE;
	}

	return FALSE;
}
// 开始采样
BOOL Protocol_SetSlaveStart(BOOL* isSlaveRun)
{
	if(sProtocol_SendNormalPacket(DPAK_CMD_WORK_START, RecvFlag_WorkStart)) {
		*isSlaveRun = (BOOL)(commStatus.DestState == WorkState_Running);
		return TRUE;
	}

	return FALSE;
}
// 终止采样
BOOL Protocol_SetSlaveAbort(BOOL* isSlaveStoped)
{
	if(sProtocol_SendNormalPacket(DPAK_CMD_WORK_ABORT, RecvFlag_WorkAbort)) {
		*isSlaveStoped = (BOOL)(commStatus.DestState == WorkState_Stopped);
		return TRUE;
	}

	return FALSE;
}
// 获取从机工作状态
WorkState Protocol_GetSlaveWorkState()
{
	if(sProtocol_SendNormalPacket(DPAK_CMD_WORK_GETSTATUS, RecvFlag_WorkState))
		return commStatus.DestState;

	return WorkState_UnKnow;
}

// 发送电抗参数
BOOL Protocol_SendReactParams(ReactParam* reactParam)
{
	UInt16 tryTime = PROTOCOL_RETRY_TIMES;
	UInt8* pBinary = (UInt8 *)reactParam;

	while(tryTime--) {
		UART_SendDataPacket(destAddr, DPAK_CMD_DAT_SETREACT, pBinary, 4);

		if(sProtocol_WaitForSlaveAck(RecvFlag_ReactParam))
			return TRUE;
	}

	return FALSE;
}
