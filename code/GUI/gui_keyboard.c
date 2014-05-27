/*************************************************************************
#    FileName: gui_keyboard.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-04 15:14:06
*************************************************************************/
#include <stdio.h>
#include "lcddrive.h"
#include "gui_basic.h"
#include "gui_keyboard.h"
volatile uint8_t vk_msg;
/* Includes ------------------------------------------------------------*/
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/

/**
  * @brief  1:press 0:lossen.
  * @param  None.
  * @retval None.
  * @verify .
  */
#define VK_DOWN_MASK 0X08
#define VK_BACK_MASK 0x04
#define VK_ENTER_MASK 0X20
#define VK_SYSSET_MASK (VK_BACK_MASK|VK_DOWN_MASK)
uint8_t _GUI_Listen_Down (void)
{
	if (SSD1906_GetGpioStatus ()&VK_DOWN_MASK){
		return 0;				//未按下
	}else{
		return 1;				//按下
	}
}
uint8_t _GUI_Listen_Back (void)
{
	if (SSD1906_GetGpioStatus ()&VK_BACK_MASK){
		return 0;
	}else{
		return 1;
	}
}
uint8_t _GUI_Listen_Enter (void)
{
	if (SSD1906_GetGpioStatus ()&VK_ENTER_MASK){
		return 0;
	}else{
		return 1;
	}
}
uint8_t _GUI_Listen_Sysset (void)
{
	if (SSD1906_GetGpioStatus ()&VK_SYSSET_MASK){
		return 0;
	}else{
		return 1;
	}
}
uint8_t _GUI_Listen_Keymsg (void)
{
	uint8_t keymsg;
	keymsg = SSD1906_GetGpioStatus ();
	return keymsg;
}
void GUI_Send_Keymsg (uint8_t VK_MSG)
{
	vk_msg = VK_MSG;
}
uint8_t GUI_Get_Keymsg (void)
{
	uint8_t tmp;
	tmp = vk_msg;
	vk_msg = VK_NOKEY;
	return tmp;
}
uint8_t GUI_Get_Keymsg_a (void)
{
	return vk_msg;
}
#include "../app/menu/usermenu.h"
volatile long speak_jif;
extern volatile long jiffies;
extern volatile uint8_t vk_msg;
#define POWER_DOWN_TIME 200	//设置关机延时
extern void BSP_PowerDown (void);
void GUI_Listen_Key (void)
{
	static uint16_t delay_limit_down = DELAY_VK1;
	static uint16_t delay_limit_back = DELAY_VK1;
	static uint16_t delay_limit_enter = DELAY_VK1;
	static uint16_t delay_limit_sysset = DELAY_VK4;//修正
	static uint16_t delay_down = 0;
	static uint16_t delay_back = 0;
	static uint16_t delay_enter = 0;
	static uint16_t delay_sysset = 0;
	uint16_t delay = 0;
	int16_t x;
	uint8_t keymsg;
	keymsg = _GUI_Listen_Keymsg ();
	/* 向下按键检测 */
	if (~keymsg&VK_DOWN_MASK){//(_GUI_Listen_Down ()){
		delay_down ++;
	}else{
		delay_down = 0;
		if (delay_limit_down > DELAY_VK1){				//松开判断
			if(delay_limit_down <= DELAY_VK1+DELAY_VK2){
				GUI_Send_Keymsg(VK_DOWN|VK_LOOSE);
			}
		}
		delay_limit_down = DELAY_VK1;
	}
	/* ----------- */
	if (~keymsg&VK_BACK_MASK){//(_GUI_Listen_Back ()){
		delay_back ++;
	}else{
		delay_back = 0;
		if (delay_limit_back > DELAY_VK1){
			if(delay_limit_back <= DELAY_VK1+DELAY_VK2){
				GUI_Send_Keymsg(VK_BACK|VK_LOOSE);		
			}
		}
		delay_limit_back = DELAY_VK1;
	}
	/* ---------- */
	if (~keymsg&VK_ENTER_MASK){//(_GUI_Listen_Enter ()){
		delay_enter ++;
	}else{
		delay_enter = 0;
		if (delay_limit_enter > DELAY_VK1){
			if(delay_limit_enter <= DELAY_VK1+DELAY_VK2){
				GUI_Send_Keymsg(VK_ENTER|VK_LOOSE);
			}
		}
		delay_limit_enter = DELAY_VK1;
	}
	/* ---------- */
	if (~keymsg&VK_BACK_MASK && ~keymsg&VK_DOWN_MASK){//(_GUI_Listen_Sysset ()){
		delay_sysset ++;
	}else{
		delay_sysset = 0;
		delay_limit_sysset = DELAY_VK4;
	}
	/* 以下有点优先级，谁比较牛逼，谁就在后面 */
	if (delay_down > delay_limit_down){
		if (delay_limit_down == DELAY_VK1){
			delay_limit_down += DELAY_VK2;
			GUI_Send_Keymsg(VK_DOWN);
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak();
				speak_jif = jiffies + 11;
			}
		}else if (delay_limit_down >= DELAY_VK2+DELAY_VK1){
			delay_limit_down += DELAY_VK3;
			GUI_Send_Keymsg(VK_DOWN|VK_CONT);
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak();
				speak_jif = jiffies + 11;
			}
		}
	}
	if (delay_enter > delay_limit_enter){
		if (delay_limit_enter == DELAY_VK1){
			delay_limit_enter += DELAY_VK2;
			GUI_Send_Keymsg(VK_ENTER);
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak();
				speak_jif = jiffies + 11;
			}
		}else if (delay_limit_enter >= DELAY_VK2+DELAY_VK1){
			delay_limit_enter += DELAY_VK3;
			GUI_Send_Keymsg(VK_ENTER|VK_CONT);
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak();
				speak_jif = jiffies + 11;
			}
		}
	}
	if(delay_enter>POWER_DOWN_TIME){
		BSP_PowerDown();
	}
	if (delay_back > delay_limit_back){
		if (delay_limit_back == DELAY_VK1){
			delay_limit_back += DELAY_VK2;
			GUI_Send_Keymsg(VK_BACK);
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak();
				speak_jif = jiffies + 11;
			}
		}else if (delay_limit_back >= DELAY_VK2+DELAY_VK1){
			delay_limit_back += DELAY_VK3;
			GUI_Send_Keymsg(VK_BACK|VK_CONT);
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak();
				speak_jif = jiffies + 11;
			}
		}
	}
	if (delay_sysset > delay_limit_sysset){
		GUI_Send_Keymsg(VK_SYSSET);
	}
	/* ------------------- */
}

/*********************************END OF FILE****************************/

