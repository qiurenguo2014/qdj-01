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
	uint16_t sbr;
	uint8_t brfa;
	uint32_t clock;
	SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	UART_PORT->PCR[4] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[4] |= PORT_PCR_MUX(3);
	UART_PORT->PCR[5] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[5] |= PORT_PCR_MUX(3);
	clock = CPUInfo.BusClock;//UART0 UART1 use CoreClock;
	sbr = (uint16_t) ((clock)/(115200ul*16));
	brfa = ((clock*2)/(115200ul-sbr*32));
	UART->BDH |= ((sbr>>8)&UART_BDH_SBR_MASK);
	UART->BDL = (sbr&UART_BDL_SBR_MASK);
	UART->C4 |= brfa & (UART_BDL_SBR_MASK>>3);
	UART->C2 &= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);//forbit send and rece
	UART->C1 &= ~UART_C1_M_MASK;//set data bits is 8bit
	UART->C1 &= ~UART_C1_PE_MASK;//set no odd-even check
	UART->S2 &= ~UART_S2_MSBF_MASK;//LSB IS start
	//UART->RWFIFO = 0Xff;
	//UART->TWFIFO = 0XFF;
	UART->C2 |= (UART_C2_RE_MASK| UART_C2_TE_MASK);
	UART->C2 |= UART_C2_RIE_MASK;
	NVIC_EnableIRQ(UART3_RX_TX_IRQn);
}
//#define UART UART4
#define UART_PORT PORTE
void UART4_Configuration (void)
{
	uint16_t sbr;
	uint8_t brfa;
	uint32_t clock;
	//printf("\r\nc0");
	SIM->SCGC4 |= SIM_SCGC1_UART4_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	UART_PORT->PCR[24] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[24] |= PORT_PCR_MUX(3);
	UART_PORT->PCR[25] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[25] |= PORT_PCR_MUX(3);
	
	clock = 50000000l;//UART0 UART1 use CoreClock;
	sbr = (uint16_t) ((clock)/(9600ul*16));
	brfa = ((clock*2)/(9600ul-sbr*32));
	//printf("\r\nc1");
	UART4->BDH |= ((sbr>>8)&UART_BDH_SBR_MASK);
	//printf("\r\nc2");
	UART4->BDL = (sbr&UART_BDL_SBR_MASK);
	//printf("\r\nc3");
	UART4->C4 |= brfa & (UART_BDL_SBR_MASK>>3);
	UART4->C2 &= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);//forbit send and rece
	UART4->C1 &= ~UART_C1_M_MASK;//set data bits is 8bit
	UART4->C1 &= ~UART_C1_PE_MASK;//set no odd-even check
	UART4->S2 &= ~UART_S2_MSBF_MASK;//LSB IS start
	UART4->C2 |= (UART_C2_RE_MASK| UART_C2_TE_MASK);
	UART4->C2 |= UART_C2_RIE_MASK;
	NVIC_EnableIRQ(UART4_RX_TX_IRQn);
}
/*********************************END OF FILE****************************/


