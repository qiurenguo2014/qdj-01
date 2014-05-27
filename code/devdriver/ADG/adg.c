/*========================================================================
#   FileName: adg.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-21 11:06:10
========================================================================*/
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "../../bsp/fwlib/gpio.h"
#include "adg.h"
/* Typedef -------------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
ADG_StructType ADG_Struct;
/* Macro ---------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Function prototypes -------------------------------------------------*/
static void GPIO_Configuration (void);
/* Functions -----------------------------------------------------------*/

void ADG_Configuration (void)
{
	GPIO_Configuration ();
	ADG_SetChannel (ADG_EARTHRES_CHANNEL);
}

/**
  * @brief  chn1 正负极  chn2 电流 1x交流电压  chn3 51x交流电压 chn4 201x交流电压.
  * @param  None
  * @retval None
  */
void ADG_SetChannel (ADG_CHANNEL_n chn)
{
	switch(chn){
		case ADG_CHANNEL_1:
			GPIO_ResetBits (ADG_A0,ADG_A0_Pin);
			GPIO_ResetBits (ADG_A1,ADG_A1_Pin);
			ADG_Struct.CHN = chn;
			break;
		case ADG_CHANNEL_2:
			GPIO_SetBits (ADG_A0,ADG_A0_Pin);
			GPIO_ResetBits (ADG_A1,ADG_A1_Pin);
			ADG_Struct.CHN = chn;
			break;
		case ADG_CHANNEL_3:
			GPIO_ResetBits (ADG_A0,ADG_A0_Pin);
			GPIO_SetBits (ADG_A1,ADG_A1_Pin);
			ADG_Struct.CHN = chn;
			break;
		case ADG_CHANNEL_4:
			GPIO_SetBits (ADG_A0,ADG_A0_Pin);
			GPIO_SetBits (ADG_A1,ADG_A1_Pin);
			ADG_Struct.CHN = chn;
			break;
		default:
			break;
	}
}
void ADG_SetChannel_DC (void)
{
	ADG_SetChannel (ADG_CHANNEL_1);
}
void ADG_SetChannel_AC (void)
{
	ADG_SetChannel (ADG_CHANNEL_2);
}
static void GPIO_Configuration (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIOx = ADG_A0;
	GPIO_InitStruct.GPIO_InitState = Bit_SET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = ADG_A0_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OOD;
	GPIO_Init (&GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIOx = ADG_A1;
	GPIO_InitStruct.GPIO_Pin = ADG_A1_Pin;
	GPIO_Init (&GPIO_InitStruct);
	
	GPIO_SetBits (ADG_A0, ADG_A0_Pin);
	GPIO_SetBits (ADG_A1, ADG_A1_Pin);
}
