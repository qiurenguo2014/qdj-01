/*************************************************************************
#    FileName: GUI_StockC.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 颜色设置和获取
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "gui_stockc.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* 定义十进制(0-7)==>十六进制位转换表，由于显示点数据是由左到右，
 * 所以十六进制位顺序是倒的 */
uint8_t const  DCB2HEX_TAB[8] = 
{0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
/* Variables -----------------------------------------------------------*/
/* 定义前景色及背景色变量，用于ASCII码、汉字、窗口、单色位图显示 */
TCOLOR  disp_color;
TCOLOR	back_color;
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  GUI_SetColor(TCOLOR color1, TCOLOR color2)
{  
	GUI_CopyColor(&disp_color, color1);
	GUI_CopyColor(&back_color, color2);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  GUI_GetBackColor(TCOLOR  *bakc)
{  GUI_CopyColor(bakc, back_color);  
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  GUI_GetDispColor(TCOLOR  *bakc)
{  GUI_CopyColor(bakc, disp_color);  
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  GUI_ExchangeColor(void)
{  
	TCOLOR  bakc;
	GUI_CopyColor(&bakc, disp_color);
	GUI_CopyColor(&disp_color, back_color);
	GUI_CopyColor(&back_color, bakc);
}
