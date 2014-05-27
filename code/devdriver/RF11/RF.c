/**********************************************************************
* @File    : 	RF.cpp
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.06
* @Brief   : 	无线模块与触发器驱动
***********************************************************************/
#include "RF.h"
#include "CRC.h"
#include "Protocol.h"

#define _ENABLE_CRC_

static DataPak_t dataPacket;

//* Private ---------------------------------------------------------*//

//================================
// 初始化配置
//================================
static void sGPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// 配置 RST 为输出口
	GPIO_InitStruct.GPIOx = RF_RST_PORT;
	GPIO_InitStruct.GPIO_InitState = Bit_SET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = RF_RST_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_Init(&GPIO_InitStruct);
	// 配置 SLP 为输出口
	GPIO_InitStruct.GPIOx = RF_SLP_PORT;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_Pin = RF_SLP_PIN;
	GPIO_Init(&GPIO_InitStruct);

}
static void sUART_Configuration()
{
	UART_InitTypeDef UART_InitStruct1;
	UART_InitStruct1.UARTxMAP = RF_UART_MAP;
	UART_InitStruct1.UART_BaudRate = RF_TRANS_BAUDRATE;
	UART_Init(&UART_InitStruct1);
}

static void UART_SendInt8(UInt8 byte)
{
	UART_SendByte(byte);
#ifdef _ENABLE_CRC_
	CRC_AddValue(byte);
#endif
}

static void UART_SendInt16(UInt16 intValue)
{
	UART_SendInt8(intValue);
	UART_SendInt8(intValue >> 8);
}

//* Public ---------------------------------------------------------*//

//================================
// 初始化无线模块
//================================
void RF_Init()
{
	//初始化端口
	sUART_Configuration();
	sGPIO_Configuration();

	//开启串口接收中断
	UART_ITConfig(RF_UART_PORT, UART_IT_RDRF, ENABLE);
	NVIC_EnableIRQ(RF_UART_IRQ);
}

void RF_SetPowerWakeUp()
{
	GPIO_ResetBits(RF_SLP_PORT, RF_SLP_PIN);
}

void RF_SetPowerSleep()
{
	GPIO_SetBits(RF_SLP_PORT, RF_SLP_PIN);
}

//================================
// 接收数据包
//================================
void RF_ReceiveData(UInt8 recvByte)
{
	static UInt16 recvLength = 0;
	static UInt8* pRecvData = (UInt8 *)&dataPacket;

	if(recvLength > DPAK_HEADER_SIZE) {
		*(pRecvData + recvLength) = recvByte;
		recvLength++;

		if(recvLength == dataPacket.PacketLength) {
#ifdef _ENABLE_CRC_
			UInt16 availLength = recvLength - 2;
			UInt16 calcCRC = CRC_CalcCRC16(pRecvData, availLength);
			const UInt16* pakCRC = (UInt16 *)(pRecvData + availLength);

			if(calcCRC == *pakCRC) {
				Protocol_AnalyzePacket(&dataPacket);
			}

#else
			Protocol_AnalyzePacket(&dataPacket);
#endif
			recvLength = 0;

		}
	} else {
		if((recvLength == 0) && (recvByte != DPAK_SYNC_HEAD1)) {
			recvLength = 0;
		} else if((recvLength == 1) && (recvByte != DPAK_SYNC_HEAD2)) {
			recvLength = 0;
		} else if((recvLength == 2) && (recvByte != DPAK_PRODUCE_ID)) {
			recvLength = 0;
		} else if((recvLength == 3) && (recvByte != DPAK_ADDR_MYSELF)) {
			recvLength = 0;
		} else if((recvLength == 5) && (dataPacket.PacketLength > sizeof(DataPak_t))) {
			recvLength = 0;
		}

		*(pRecvData + recvLength) = recvByte;
		recvLength++;
	}

}

//================================
// 发送数据包
//================================
void RF_SendDataPacket(UInt8 addr, UInt8 cmd, UInt8* binArray, UInt16 binLength)
{
	UInt16 crcValue = 0;
	UInt16 pakLength = binLength + sizeof(DataPak_t) - DPAK_BINARY_SIZE;

#ifdef _ENABLE_CRC_
	CRC_CreateNew();
#endif

	// SyncHeader
	UART_SendInt8(DPAK_SYNC_HEAD1);
	UART_SendInt8(DPAK_SYNC_HEAD2);
	// ProduceID
	UART_SendInt8(DPAK_PRODUCE_ID);
	// DestinAddr
	UART_SendInt8(addr);
	// PacketLength
	UART_SendInt16(pakLength);
	// SourceAddr
	UART_SendInt8(DPAK_ADDR_MYSELF);
	// Command
	UART_SendInt8(cmd);

	// BinData
	while(binLength--) {
		UART_SendInt8(*binArray++);
	}

	// CRC16
#ifdef _ENABLE_CRC_
	crcValue = CRC_GetCRC16();
#endif

	UART_SendInt16(crcValue);

}
