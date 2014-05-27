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

/* Exported types ------------------------------------------------------*/
#define UART_TX_BUF_SIZE 0xff
#define UART_RX_BUF_SIZE 0xff
typedef struct{
	u8 TxBuf[UART_TX_BUF_SIZE];
	u8 RxBuf[UART_RX_BUF_SIZE];
	u32 IsComplete;
	u32 Txprecounter;
	u32 Rxprecounter;
	u32 Txcounter;//send counter
	u32 Rxcounter;//receive counter
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


