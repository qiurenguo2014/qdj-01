/*========================================================================
#   FileName: tim.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-22 14:02:11
========================================================================*/
/* Includes ---------------------------------------------------------*/

#include "tim.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
#define PITX PIT0
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
/* Functions --------------------------------------------------------*/
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void PIT_Configuration (void)
{
	PIT_InitTypeDef PIT_InitStruct;
	  //初始化PIT
	PIT_InitStruct.PITx = PITX;          //PIT0通道
	PIT_InitStruct.PIT_Interval = 1000;   //定时周期1000MS
	PIT_Init(&PIT_InitStruct);
	PIT_StopTim ();
	PIT_ITConfig(PIT0,PIT_IT_TIF,ENABLE);	
	NVIC_EnableIRQ(PIT0_IRQn);

}

void PIT_EnableIRQ(void)
{
	NVIC_EnableIRQ (PIT0_IRQn);
}

void PIT_DisableIRQ(void)
{
	NVIC_EnableIRQ (PIT0_IRQn);
}

void PIT_StartTim (void)
{
	PIT->CHANNEL[PITX].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_StopTim (void)
{
	PIT->CHANNEL[PITX].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

/*********************************END OF FILE**********************************/

