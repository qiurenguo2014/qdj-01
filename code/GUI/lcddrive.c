/*************************************************************************
#    FileName: lcddrive.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "gui_font.h"
#include "macro.h"
#include "lcddrive.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/

/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_FillSCR (TCOLOR dat)
{
	register uint32_t i;	
	i = 76800;
	while (--i){
		pSSD1906DAT[i] = dat;
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_ClearSCR (void)
{
	register uint32_t i;
	i = 76800;
	while (--i){
		pSSD1906DAT[i] = BLACK;
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_Point (int16_t x, int16_t y, TCOLOR color)
{
	int32_t offset;
	if (x < GUI_LCM_XMAX && y < GUI_LCM_YMAX){
		offset = y*GUI_LCM_XMAX+x;
		pSSD1906DAT[offset] = color;
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
int API_GUI_ReadPoint (int16_t x, int16_t y, TCOLOR *ret)
{
	int16_t addr;
	if(x>=GUI_LCM_XMAX) return(0);
	if(y>=GUI_LCM_YMAX) return(0);
	addr = y*GUI_LCM_XMAX+x;
	*ret = pSSD1906DAT[addr];
	return(1);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_HLine (int16_t x0, int16_t y0, int16_t x1, TCOLOR color)
{
	int16_t  bak;
	if(x0>x1){
		bak = x1;
		x1 = x0;
		x0 = bak;
	}
	if(x0==x1){
		API_GUI_Point(x0, y0, color);
		return;
	}
	do{
		API_GUI_Point(x0, y0, color);
		x0++;
	}while(x1>x0);
	API_GUI_Point(x0, y0, color);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_RLine (int16_t x0, int16_t y0, int16_t y1, TCOLOR color)
{
	int16_t  bak;
	if(y0>y1){
		bak = y1;
		y1 = y0;
		y0 = bak;
	}
	if(y0==y1){
		API_GUI_Point(x0, y0, color);
		return;
	}
	do{
		API_GUI_Point(x0, y0, color);
		y0++;
	}while(y1>y0);
	API_GUI_Point(x0, y0, color);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_OpenBackLight (void)
{
	SSD1906_OpenBackLight ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_CloseBackLight (void)
{
	SSD1906_CloseBackLight ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_OpenSpeak (void)
{
	SSD1906_openSpeak ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_CloseSpeak (void)
{
	SSD1906_closeSpeak ();
}
/**
  * @brief  调节背光亮度
  * 		分四级
  * 		0~3
  * 		0最暗.
  * @param  None.
  * @retval None.
  * @verify .
  */
void API_GUI_SetBackLightLevel (uint8_t level)
{
	switch(level){
		case 0:
			SSD1906_SetBacklightLevel (0);
			break;
		case 1:
			SSD1906_SetBacklightLevel (100);
			break;
		case 2:
			SSD1906_SetBacklightLevel (150);
			break;
		case 3:
			SSD1906_SetBacklightLevel (240);
			break;
		default:
			SSD1906_SetBacklightLevel (0);
			break;
	}
}
/*********************************END OF FILE****************************/


