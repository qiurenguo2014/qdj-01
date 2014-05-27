/**********************************************************************
* @File    : 	RF.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.06
* @Brief   : 	无线模块与触发器驱动
***********************************************************************/
#ifndef __RF_H_
#define __RF_H_

// Include
//#include "DataType.h"
#include "datapacket.h"
#include "../../bsp/fwlib/GPIO.h"
#include "../../bsp/fwlib/UART.h"

// Pin Map
#define RF_RST_PORT						PTA
#define RF_RST_PIN						GPIO_Pin_12
#define RF_SLP_PORT						PTA
#define RF_SLP_PIN						GPIO_Pin_13
#define RF_UART_PORT					UART4
#define RF_UART_MAP						UART4_RX_E24_TX_E25
#define RF_UART_IRQ						UART4_RX_TX_IRQn

// Define
#define RF_TRANS_BAUDRATE				9600
#define RF_TRANS_TIMEOUT				500
#define RF_TRANS_RETRY					3

#define UART_SendByte(x)				UART_SendData(RF_UART_PORT, x)
#define UART_ReceiveByte(x)				UART_ReceiveData(RF_UART_PORT, x)
#define UART_SendDataPacket(addr, cmd, array, length)	\
										RF_SendDataPacket(addr, cmd, array, length)

// Public

void RF_Init();
void RF_SetPowerWakeUp();
void RF_SetPowerSleep();
void RF_SendDataPacket(UInt8 addr, UInt8 cmd, UInt8* binArray, UInt16 binLength);
void RF_ReceiveData(UInt8 recvByte);

#endif
