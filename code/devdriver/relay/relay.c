/*========================================================================
#   FileName: relay.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-21 16:31:32
========================================================================*/
/* Includes ------------------------------------------------------------*/
#include "stdint.h"
#include "../../bsp/fwlib/gpio.h"
#include "relay.h"
RELAY_StructType relay;
extern long volatile jiffies;	//10ms add 1
/* Typedef -------------------------------------------------------------*/
/* Define --------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Function prototypes -------------------------------------------------*/
static void GPIO_Configuration (void);
/* Functions -----------------------------------------------------------*/

void RL_Configuration (void)
{
	GPIO_Configuration ();
	RL_ZRelayDisconnect ();
	RL_FRelayDisconnect ();
//	RL_ZRelayConnect ();
//	RL_FRelayConnect ();
}

/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RL_ZRelayHiZ (void)
{
	GPIO_ResetBits (Z_RL1, Z_RL1_Pin);
	GPIO_ResetBits (Z_RL2, Z_RL2_Pin);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RL_FRelayHiZ (void)
{
	GPIO_ResetBits (F_RL1, F_RL1_Pin);
	GPIO_ResetBits (F_RL2, F_RL2_Pin);
}
/**
  * @brief  Ê≠£Á ˝Ê∫êÁªßÁîµÂô®Êé•ÈÄ?ÁªßÁ ˝Âô®Âä®‰Ω≤◊ó∂Èó¥Â§ßÁ∫?ms
  * @param  None
  * @retval None
  */
void RL_ZRelayConnect (void)
{
	GPIO_SetBits (Z_RL2, Z_RL2_Pin);
	GPIO_ResetBits (Z_RL1, Z_RL1_Pin);
	relay.status |= RELAY_STTS_UPDATA;
	relay.jif_value = jiffies + RELAY_Hiz_jif;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RL_ZRelayDisconnect (void)
{
	GPIO_SetBits (Z_RL1, Z_RL1_Pin);
	GPIO_ResetBits (Z_RL2, Z_RL2_Pin);
	relay.status |= RELAY_STTS_UPDATA;
	relay.jif_value = jiffies + RELAY_Hiz_jif;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RL_FRelayConnect (void)
{
	GPIO_ResetBits (F_RL1, F_RL1_Pin);
	GPIO_SetBits (F_RL2, F_RL2_Pin);
	relay.status |= RELAY_STTS_UPDATA;
	relay.jif_value = jiffies + RELAY_Hiz_jif;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RL_FRelayDisconnect (void)
{
	GPIO_ResetBits (F_RL2, F_RL2_Pin);
	GPIO_SetBits (F_RL1, F_RL1_Pin);
	relay.status |= RELAY_STTS_UPDATA;
	relay.jif_value = jiffies + RELAY_Hiz_jif;
}
static void GPIO_Configuration (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIOx = Z_RL1;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = Z_RL1_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_Init (&GPIO_InitStruct);

	GPIO_InitStruct.GPIOx = Z_RL2;
	GPIO_InitStruct.GPIO_Pin = Z_RL2_Pin;
	GPIO_Init (&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIOx = F_RL1;
	GPIO_InitStruct.GPIO_Pin = F_RL1_Pin;
	GPIO_Init (&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIOx = F_RL2;
	GPIO_InitStruct.GPIO_Pin = F_RL2_Pin;
	GPIO_Init (&GPIO_InitStruct);
}

