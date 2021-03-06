/*========================================================================
#   FileName: systick.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-22 16:06:35
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdint.h>
#include "..\FWLIB\sys.h"
#include "systick.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/

SYSTICK_StructType Systick;
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
long volatile jiffies=0;	//10ms add 1
/* Functions --------------------------------------------------------*/

/**
  * @brief  .
  * @param  None
  * @retval None
  */
void STK_Configuration (void)
{
	SysTick->LOAD  = CPUInfo.CoreClock/CLOCKS_PER_SEC - 1;     /* set reload register */
	SysTick->VAL   = 0;                                        /* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
					SysTick_CTRL_ENABLE_Msk;                   /* Enable SysTick IRQ and SysTick Timer */
	
	jiffies = 0;
	Systick.jiffies[JIFFIES_WDOG] = jiffies + CLOCKS_PER_SEC * 5;	
//	NVIC_EnableIRQ(SysTick_IRQn);
}

void STK_delay10ms (long jif)
{
	long jifb;
	jifb = jiffies + jif;
	while (jifb >= jiffies);
}
void STK_delay1ms (long ms)
{
//	uint32_t temp;
//	uint16_t i;
//	for(i=0;i<ms;i++){
//		SysTick->LOAD=ms;  					 
//		SysTick->VAL=0x00;
//		SysTick->CTRL|=0x01;  
//		do{
//			temp=SysTick->CTRL;
//		}
//		while(temp&0x01&&!(temp&(1<<16)));
//		SysTick->CTRL&=~0x01; 
//	}
}
/*********************************END OF FILE**********************************/


