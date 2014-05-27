/**********************************************************************
* @File    : 	DataPacket.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.12.10
* @Brief   : 	通讯数据相关
***********************************************************************/
#ifndef _DATAPACKET_H_
#define _DATAPACKET_H_

// Include
//#include "DataType.h"
typedef unsigned short UInt16;
typedef unsigned char UInt8;
typedef unsigned int UInt32;
typedef int Int32;
typedef unsigned char BOOL;
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif
// Define
#define DPAK_SYNC_HEAD1					0xFF
#define DPAK_SYNC_HEAD2					0x55
#define DPAK_PRODUCE_ID					0x28
#define DPAK_ADDR_MYSELF				0x01		//本机地址【主机01 手持器02】
#define DPAK_ADDR_DEST					0x02		//目的地址【主机01 手持器02】


// Command
#define DPAK_CMD_SYS_PING				0xA0
#define DPAK_CMD_SYS_ACK				0xA1

#define DPAK_CMD_CALIB_REQ				0xB0
#define DPAK_CMD_CALIB_CONFIRM			0xB1
#define DPAK_CMD_CALIB_ABORT			0xB2
#define DPAK_CMD_CALIB_ABORT_CONFIRM	0xB3

#define DPAK_CMD_WORK_READY				0xC0
#define DPAK_CMD_WORK_READY_CONFIRM		0xC1
#define DPAK_CMD_WORK_START				0xC2
#define DPAK_CMD_WORK_START_CONFIRM		0xC3
#define DPAK_CMD_WORK_ABORT				0xC4
#define DPAK_CMD_WORK_ABORT_CONFIRM		0xC5
#define DPAK_CMD_WORK_GETSTATUS			0xC6
#define DPAK_CMD_WORK_RETSTATUS			0xC7
#define DPAK_CMD_WORK_ERROR				0xCF

#define DPAK_CMD_DAT_SETREACT			0xD0
#define DPAK_CMD_DAT_RETREACT			0xD1


// Struct
#define DPAK_BINARY_SIZE				256
#define DPAK_HEADER_SIZE				7

typedef struct {
	UInt16	SyncHeader;					//同步头

	UInt8	ProduceID;					//产品ID
	UInt8	DestinAddr;					//目的地址
	UInt16	PacketLength;				//数据包全长
	UInt8	SourceAddr;					//源地址

	UInt8	Command;					//命令代码
	UInt8	BinData[DPAK_BINARY_SIZE];	//二进制数据

	UInt16	CRC16;						//CRC16
}DataPak_t;


#endif
