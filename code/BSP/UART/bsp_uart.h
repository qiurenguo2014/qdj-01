/*========================================================================
#   FileName: uart.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-16 16:37:35
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __UART_H
#define __UART_H
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "..\FWLIB\sys.h"
/* Exported types ------------------------------------------------------*/
#define UART_TX_BUF_SIZE 0xff
#define UART_RX_BUF_SIZE 0xff
typedef struct{
	uint8_t TxBuf[UART_TX_BUF_SIZE];
	uint8_t RxBuf[UART_RX_BUF_SIZE];
	uint32_t IsComplete;
	uint32_t Txprecounter;
	uint32_t Rxprecounter;
	uint32_t Txcounter;//send counter
	uint32_t Rxcounter;//receive counter
}UART_MSGSTRUCTTYPE;
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
#define UART UART3
#define UART_PORT PORTE
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern void UART_Configuration (void);
#endif 
/*********************************END OF FILE****************************/


