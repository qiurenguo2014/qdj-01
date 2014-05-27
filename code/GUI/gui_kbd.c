/*************************************************************************
#    FileName: gui_kbd.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-26 16:41:20
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "GUI_KBD.H"
#include "..\..\bsp\ssd1906\ssd1906_hw_fb16b.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
KBD_StructType KBD_Struct;
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void KBD_Init (void)
{
	KBD_Struct.msg = 0;
	KBD_Struct.msg_u = 0;
}
/**
  * @brief  1:press 0:lossen.
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 _KBD_Listen_Down (void)
{
	if (SSD1906_GetGpioStatus ()&KBD_DOWN_BIT){
		return 0;
	}else{
		return 1;
	}
}
u8 _KBD_Listen_Up (void)
{
	if (SSD1906_GetGpioStatus ()&KBD_UP_BIT){
		return 0;
	}else{
		return 1;
	}
}
u8 _KBD_Listen_Enter (void)
{
	if (SSD1906_GetGpioStatus ()&KBD_ENTER_BIT){
		return 0;
	}else{
		return 1;
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void KBD_ReadKey_u (void)
{
	if (_KBD_Listen_Up ()){
		if ((KBD_Struct.msg_u & KBD_MSG_UP) == KBD_MSG_UP){
			KBD_Struct.msg &= (~KBD_MSG_UP);
		}else{
			KBD_Struct.msg |= KBD_MSG_UP;
			KBD_Struct.msg_u |= KBD_MSG_UP;
		}
	}else{
		KBD_Struct.msg_u &= ~KBD_MSG_UP;
		KBD_Struct.msg &= ~KBD_MSG_UP;
	}
	if (_KBD_Listen_Down ()){
		if ((KBD_Struct.msg_u & KBD_MSG_DOWN) == KBD_MSG_DOWN){
			KBD_Struct.msg &= (~KBD_MSG_DOWN);
		}else{
			KBD_Struct.msg |= KBD_MSG_DOWN;
			KBD_Struct.msg_u |= KBD_MSG_DOWN;
		}
	}else{
		KBD_Struct.msg_u &= ~KBD_MSG_DOWN;
		KBD_Struct.msg &= ~KBD_MSG_DOWN;
	}
	if (_KBD_Listen_Enter ()){
		if ((KBD_Struct.msg_u & KBD_MSG_ENTER) == KBD_MSG_ENTER){
			KBD_Struct.msg &= (~KBD_MSG_ENTER);
		}else{
			KBD_Struct.msg |= KBD_MSG_ENTER;
			KBD_Struct.msg_u |= KBD_MSG_ENTER;
		}
	}else{
		KBD_Struct.msg_u &= ~KBD_MSG_ENTER;
		KBD_Struct.msg &= ~KBD_MSG_ENTER;
	}
}

/*********************************END OF FILE****************************/


