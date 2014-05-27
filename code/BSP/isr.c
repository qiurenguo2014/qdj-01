/*========================================================================
#   FileName: isr.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-22 15:54:31
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdint.h>
#include "systick\systick.h"
#include "../startup/MK60D10.h"
#include "../gui/gui_keyboard.h"
#include "../gui/lcddrive.h"
#include "../devdriver\AD7687\ad7687.h"
#include "../app/signal/sig_process.h"
#include "isr.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
/* Functions --------------------------------------------------------*/

/**
  * @brief  .
  * @param  None
  * @retval None
  */
extern volatile long speak_jif;
void SysTick_Handler (void)
{
	jiffies ++;
	GUI_Listen_Key ();
	if (speak_jif < jiffies){
		API_GUI_CloseSpeak ();
	}else{
	}
	//NVIC_ClearPendingIRQ (SysTick_IRQn);
}
void RTC_IRQHandler (void)
{
}
void PIT0_IRQHandler(void)
{
//	if(SIGPRC_Struct.status & SIG_STAT_READYCONVER){
//		SIGPRC_Struct.status &= ~SIG_STAT_READYCONVER;
//		AD7687_StartConver ();
//	}
	printf("\r\nj:%d",jiffies);
	PIT_ClearITPendingBit(PIT0,PIT_IT_TIF);
}
void PORTA_IRQHandler (void)
{
	if(AD7687_SIN_PORT->ISFR & (1<<AD7687_SIN_Pin))
	{
		if (DEFAULT_PERIOD == 
					SIGPRC_Struct.collect_counter){
			SIG_CompleteConverCollect ();
		}else{
			SIG_GetAD7687data (sig_ad[SIGPRC_Struct.collect_counter]);
			SIGPRC_Struct.collect_counter++;
		}
		AD7687_SIN_PORT->ISFR |= (1<<AD7687_SIN_Pin);
	}
}
void UART3_RX_TX_IRQHandler (void)
{	
	uint8_t ch;
	if (UART3->S1 & UART_S1_RDRF_MASK){
		ch = UART3->D;
	}
}
#include "../app/wifi/rf.h"
extern void RF_ReceiveData(uint8_t recvByte);
void UART4_RX_TX_IRQHandler(void)
{
	uint8_t recvValue;
//	if (UART4->S1 & UART_S1_RDRF_MASK){
//		RF_ReceiveData (UART4->D);
//	}
 	if(UART_ReceiveByte(&recvValue)) {
 		RF_ReceiveData(recvValue);
 	}
}
/*********************************END OF FILE**********************************/

