/*========================================================================
#   FileName: uart.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-16 16:37:22
========================================================================*/
/* Includes ------------------------------------------------------------*/
#include "bsp_uart.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
void UART_Configuration (void)
{
	u16 sbr;
	u8 brfa;
	u32 clock;
	SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	UART_PORT->PCR[4] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[4] |= PORT_PCR_MUX(3);
	UART_PORT->PCR[5] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[5] |= PORT_PCR_MUX(3);
	clock = CPUInfo.BusClock;//UART0 UART1 use CoreClock;
	sbr = (u16) ((clock)/(9600ul*16));
	brfa = ((clock*2)/(9600ul-sbr*32));
	UART->BDH |= ((sbr>>8)&UART_BDH_SBR_MASK);
	UART->BDL = (sbr&UART_BDL_SBR_MASK);
	UART->C4 |= brfa & (UART_BDL_SBR_MASK>>3);
	UART->C2 &= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);//forbit send and rece
	UART->C1 &= ~UART_C1_M_MASK;//set data bits is 8bit
	UART->C1 &= ~UART_C1_PE_MASK;//set no odd-even check
	UART->S2 &= ~UART_S2_MSBF_MASK;//LSB IS start
	UART->C2 |= (UART_C2_RE_MASK| UART_C2_TE_MASK);
	//开启串口接收功�?
	UART->C2 |= UART_C2_RIE_MASK;
	//接通NVIC上对应串口中断线
	NVIC_EnableIRQ(UART3_RX_TX_IRQn);
}

/*********************************END OF FILE****************************/


