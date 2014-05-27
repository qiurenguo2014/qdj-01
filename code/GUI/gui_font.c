/*************************************************************************
#    FileName: gui_keyboard.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-04 15:14:06
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "asc16.h"
#include "asc24.h"
#include "asc32.h"
#include "hzk16.h"
#include "hzk24.h"
#include "hzk32.h"
#include "macro.h"
#include "gui_stockc.h"
#include "gui_font.h"
/* Includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------*/
Font_StructTypedef Font_Struct;
FONT_StructType current_font;
/* Private define ------------------------------------------------------*/
/* 定义一个字体最大占用空间 */
#define MAX_FONT_SIZE 32*4
/* Private macro -------------------------------------------------------*/
/* Private variables ---------------------------------------------------*/

/* Private function prototypes -----------------------------------------*/
/* Private functions ---------------------------------------------------*/
/**
  * @brief  use font ,must be run this function.
  * @param  None
  * @retval None
  */
void GUI_Font_Init (void)
{
 	Font_Struct.hzkfontsize[FONT_HZK_DEFAULT] = HZK16_getfontcount ();
 	Font_Struct.hzkfontsize[FONT_HZK_16] = HZK16_getfontcount ();
 	Font_Struct.hzkfontsize[FONT_HZK_24] = HZK24_getfontcount ();
 	Font_Struct.hzkfontsize[FONT_HZK_32] = HZK32_getfontcount ();
	Font_Struct.ascfontsize[FONT_ASC_DEFAULT] = ASC16_getfontcount ();
	Font_Struct.ascfontsize[FONT_ASC_16] = ASC16_getfontcount ();
	Font_Struct.ascfontsize[FONT_ASC_24] = ASC24_getfontcount ();
	Font_Struct.ascfontsize[FONT_ASC_32] = ASC32_getfontcount ();
	current_font.ascsize = FONT_ASC_DEFAULT;
	current_font.hzksize = FONT_HZK_DEFAULT;
}
void GUI_SetHzkType (FONT_HZK_TYPE hzktype)
{
	current_font.hzksize = hzktype;
}
void GUI_SetAscType (FONT_ASC_TYPE asctype)
{
	current_font.ascsize = asctype;
}
uint8_t Gui_GetFont_asc16 (const char *ptr, unsigned char *data)
{
	unsigned int i;
	const unsigned char *p = asc16;	
	for(i=2+FONT_ASC_16_SIZE; i<Font_Struct.ascfontsize[FONT_ASC_16]; i += FONT_ASC_16_SIZE+1){				//2:宽和高  2:字符数
		if(ptr[0] == p[i]){
			p = (p+i-FONT_ASC_16_SIZE);
			for(i=0; i<FONT_ASC_16_SIZE; i++){
				data[i] = p[i];
			}
			return TRUE;
		}
	}
	return FALSE;
}
uint8_t Gui_GetFont_asc24 (const char *ptr, unsigned char *data)
{
	unsigned int i;
	const unsigned char *p = asc24;	
	for(i=2+FONT_ASC_24_SIZE; i<Font_Struct.ascfontsize[FONT_ASC_24]; i += FONT_ASC_24_SIZE+1){				//2:宽和高  2:字符数
		if(ptr[0] == p[i]){
			p = (p+i-FONT_ASC_24_SIZE);
			for(i=0; i<FONT_ASC_24_SIZE; i++){
				data[i] = p[i];
			}
			return TRUE;
		}
	}
	return FALSE;
}
uint8_t Gui_GetFont_asc32 (const char *ptr, unsigned char *data)
{
	unsigned int i;
	const unsigned char *p = asc32;	
	for(i=2+FONT_ASC_32_SIZE; i<Font_Struct.ascfontsize[FONT_ASC_32]; i += FONT_ASC_32_SIZE+1){				//2:宽和高  2:字符数
		if(ptr[0] == p[i]){
			p = (p+i-FONT_ASC_32_SIZE);
			for(i=0; i<FONT_ASC_32_SIZE; i++){
				data[i] = p[i];
			}
			return TRUE;
		}
	}
	return FALSE;
}
uint8_t Gui_GetFont_hzk16 (const char *ptr, unsigned char *data)
{
	unsigned int i;
	const unsigned char *p = hzk16;	
	for(i=2; i<Font_Struct.hzkfontsize[FONT_HZK_16]; i += FONT_HZK_16_SIZE+2){				//2:宽和高  2:字符数
		if(ptr[0] == p[i]){
			if (ptr[1] == p[i+1]){
				p = (p+i+2);
				for(i=0; i<FONT_HZK_16_SIZE; i++){
					data[i] = p[i];
				}
				return TRUE;

			}
		}
	}
	return FALSE;
}
uint8_t Gui_GetFont_hzk24 (const char *ptr, unsigned char *data)
{
	unsigned int i;
	const unsigned char *p = hzk24;	
	for(i=2; i<Font_Struct.hzkfontsize[FONT_HZK_24]; i += FONT_HZK_24_SIZE+2){				//2:宽和高  2:字符数
		if(ptr[0] == p[i]){
			if (ptr[1] == p[i+1]){
				p = (p+i+2);
				for(i=0; i<FONT_HZK_24_SIZE; i++){
					data[i] = p[i];
				}
				return TRUE;

			}
		}
	}
	return FALSE;
}
uint8_t Gui_GetFont_hzk32 (const char *ptr, unsigned char *data)
{
	unsigned int i;
	const unsigned char *p = hzk32;	
	for(i=2; i<Font_Struct.hzkfontsize[FONT_HZK_32]; i += FONT_HZK_32_SIZE+2){				//2:宽和高  2:字符数
		if(ptr[0] == p[i]){
			if (ptr[1] == p[i+1]){
				p = (p+i+2);
				for(i=0; i<FONT_HZK_32_SIZE; i++){
					data[i] = p[i];
				}
				return TRUE;

			}
		}
	}
	return FALSE;
}
/**
  * @brief  由当前字体结构得到字体高度.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint16_t GUI_GetFontHzkHigh (void)
{
	uint16_t wh;
	switch(current_font.hzksize){
		case FONT_HZK_DEFAULT:
			wh = FONT_HZK_16_HIGH;
			break;
		case FONT_HZK_16:
			wh = FONT_HZK_16_HIGH;
			break;
		case FONT_HZK_24:
			wh = FONT_HZK_24_HIGH;
			break;
		case FONT_HZK_32:
			wh = FONT_HZK_32_HIGH;
			break;
		default:
			wh = 0;
			break;
	}
	return wh;
}
/**
  * @brief  由当前字体结构得到字体宽度.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint16_t GUI_GetFontHzkWith (void)
{
	uint16_t wh;
	switch(current_font.hzksize){
		case FONT_HZK_DEFAULT:
			wh = FONT_HZK_16_WITH;
			break;
		case FONT_HZK_16:
			wh = FONT_HZK_16_WITH;
			break;
		case FONT_HZK_24:
			wh = FONT_HZK_24_WITH;
			break;
		case FONT_HZK_32:
			wh = FONT_HZK_32_WITH;
			break;
		default:
			wh = 0;
			break;
	}
	return wh;
}
/**
  * @brief  由当前字体结构得到字体高度.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint16_t GUI_GetFontAscHigh (void)
{
	uint16_t wh;
	switch(current_font.ascsize){
		case FONT_ASC_DEFAULT:
			wh = FONT_ASC_16_HIGH;
			break;
		case FONT_ASC_16:
			wh = FONT_ASC_16_HIGH;
			break;
		case FONT_ASC_24:
			wh = FONT_ASC_24_HIGH;
			break;
		case FONT_ASC_32:
			wh = FONT_ASC_32_HIGH;
			break;
		default:
			wh = 0;
			break;
	}
	return wh;
}
/**
  * @brief  由当前字体结构得到字体宽度.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint16_t GUI_GetFontAscWith (void)
{
	uint16_t wh;
	switch(current_font.ascsize){
		case FONT_ASC_DEFAULT:
			wh = FONT_ASC_16_WITH;
			break;
		case FONT_ASC_16:
			wh = FONT_ASC_16_WITH;
			break;
		case FONT_ASC_24:
			wh = FONT_ASC_24_WITH;
			break;
		case FONT_ASC_32:
			wh = FONT_ASC_24_WITH;
			break;
		default:
			wh = 0;
			break;
	}
	return wh;
}
/*
* @brief  .
* @param  None.
* @retval None.
* @verify .
*/
uint16_t GUI_GetFontHzkSize (void)
{
	uint16_t s;
	switch(current_font.hzksize){
		case FONT_HZK_DEFAULT:
			s = FONT_HZK_16_SIZE;
			break;
		case FONT_HZK_16:
			s = FONT_HZK_16_SIZE;
			break;
		case FONT_HZK_24:
			s = FONT_HZK_24_SIZE;
			break;
		case FONT_HZK_32:
			s = FONT_HZK_32_SIZE;
			break;
		default:
			s = 0;
			break;
	}
	return s;
}
/*
*@brief  .
*@param  None.
*@retval None.
*@verify .
*/
uint16_t GUI_GetFontAscSize (void)
{
	uint16_t s;
	switch(current_font.ascsize){
		case FONT_ASC_DEFAULT:
			s = FONT_ASC_16_SIZE;
			break;
		case FONT_ASC_16:
			s = FONT_ASC_16_SIZE;
			break;
		case FONT_ASC_24:
			s = FONT_ASC_24_SIZE;
			break;
		case FONT_ASC_32:
			s = FONT_ASC_32_SIZE;
			break;
		default:
			s = 0;
			break;
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_GetFontData (const char *ptr, unsigned char *data)
{
	uint16_t size;
	uint8_t ret;
	if (ptr[0] <= 126){
		switch(current_font.ascsize){
			case FONT_ASC_DEFAULT:
				ret = Gui_GetFont_asc16 (ptr, data);
				break;
			case FONT_ASC_16:
				ret = Gui_GetFont_asc16 (ptr, data);
				break;
			case FONT_ASC_24:
				ret = Gui_GetFont_asc24 (ptr, data);
				break;
			case FONT_ASC_32:
				ret = Gui_GetFont_asc32 (ptr, data);
				break;
			default:				
				break;
		}
	}else{
		switch(current_font.hzksize){
			case FONT_HZK_DEFAULT:
				ret = Gui_GetFont_hzk16 (ptr, data);
				break;
			case FONT_HZK_16:
				ret = Gui_GetFont_hzk16 (ptr, data);
				break;
			case FONT_HZK_24:
				ret = Gui_GetFont_hzk24 (ptr, data);
				break;
			case FONT_HZK_32:
				ret = Gui_GetFont_hzk32 (ptr, data);
				break;
			default:				
				break;
		}
	}
	return ret;
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void GUI_PutChar (const int16_t x, const int16_t y,
					const uint16_t w, const uint16_t h,
					unsigned char *data)
{
	int16_t x0,y0;
	uint32_t i = 0;
	uint8_t mask=0;
	for (y0=y; y0<y+h; y0++){
		mask = 0;
		for (x0=x; x0 < x+w; x0++){
			if(mask== 0){
				mask = 0x80;
			}
			if ((data[i] & mask) == mask){
				GUI_Point (x0, y0, disp_color);
			}else{
				//GUI_Point (x0, y0, back_color);
			}
			mask >>= 1;
			if(mask==0){
				i++;
			}
		}
		if(mask!=0){
			i++;
		}
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void GUI_PutCharBack (const int16_t x, const int16_t y,
					const uint16_t w, const uint16_t h,
					unsigned char *data)
{
	int16_t x0,y0;
	uint32_t i = 0;
	uint8_t mask=0;
	for (y0=y; y0<y+h; y0++){
		mask = 0;
		for (x0=x; x0 < x+w; x0++){
			if(mask== 0){
				mask = 0x80;
			}
			if ((data[i] & mask) == mask){
				GUI_Point (x0, y0, disp_color);
			}else{
				GUI_Point (x0, y0, back_color);
			}
			mask >>= 1;
			if(mask==0){
				i++;
			}
		}
		if(mask!=0){
			i++;
		}
	}
}

/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void GUI_PutString (const uint16_t x, const uint16_t y, const char *ptr)
{
	uint32_t i = 0;
	uint32_t h,w;
	unsigned char *data;
	data = (unsigned char*) malloc (MAX_FONT_SIZE);
	Current_Coor.x = x;
	Current_Coor.y = y;
	while (ptr[i]){
		if (ptr[i] <= 126){
			h = GUI_GetFontAscHigh ();
			w = GUI_GetFontAscWith ();
		}else{
			h = GUI_GetFontHzkHigh ();
			w = GUI_GetFontHzkWith ();
		}
		if(ptr[i] == '\n'){
			Current_Coor.y += h;
			Current_Coor.x = x;
		}else{
			if (GUI_GetFontData (&ptr[i], data) == TRUE){
				GUI_PutChar (Current_Coor.x, Current_Coor.y,
								w, h, data);
			}else{
			}
			Current_Coor.x += w;
		}
		if (ptr[i] <= 126){
			i++;
		}else{
			i+=2;
		}
	}
	free(data);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void GUI_PutStringBack (const uint16_t x, const uint16_t y, const char *ptr)
{
	uint32_t i = 0;
	uint32_t h,w;
	unsigned char *data;
	data = (unsigned char*) malloc (MAX_FONT_SIZE);
	Current_Coor.x = x;
	Current_Coor.y = y;
	while (ptr[i]){
		/* 获得宽和高 */
		if (ptr[i] <= 126){
			h = GUI_GetFontAscHigh ();
			w = GUI_GetFontAscWith ();
		}else{
			h = GUI_GetFontHzkHigh ();
			w = GUI_GetFontHzkWith ();
		}
		if(ptr[i] == '\n'){
			Current_Coor.y += h;
			Current_Coor.x = x;
		}else{
			if (GUI_GetFontData (&ptr[i], data) == TRUE){
				GUI_PutCharBack (Current_Coor.x, Current_Coor.y,
								w, h, data);
			}else{
			}
			Current_Coor.x += w;
		}
		if (ptr[i] <= 126){
			i++;
		}else{
			i+=2;
		}
	}
	free(data);
}
void GUI_PutStringBack_Rect (const uint16_t x, const uint16_t y, const char *buf)
{
	uint32_t x1,y1;
	Current_Coor.x = x;
	Current_Coor.y = y;
	GUI_PutStringBack (Current_Coor.x,Current_Coor.y,buf);
	x1 = Current_Coor.x;
	y1 = y+GUI_GetFontAscHigh ();
	GUI_Rectangle(x,y,x1,y1,BLACK);
	Current_Coor.x += 1;
}
void GUI_PutStringBack_Rect_24 (const uint16_t x, const uint16_t y, const char *buf)
{
	uint32_t x0,y0,x1,y1;
	uint8_t count = 0;
	Current_Coor.x = x;
	Current_Coor.y = y;
	while(buf[count]){
		count++;
	}
	x0 = Current_Coor.x-2;
	y0 = Current_Coor.y-4;
	x1 = Current_Coor.x + GUI_GetFontAscWith()*count+2;
	y1 = Current_Coor.y + 4 + GUI_GetFontAscHigh();
	GUI_RectangleFill(x0,y0,x1,y1,back_color);
	GUI_Rectangle(x0,y0,x1,y1,BLACK);
	
	GUI_PutString (Current_Coor.x,Current_Coor.y,buf);
}
char printf_buf[100];
void GUI_Printf (const char *buf)
{
	GUI_PutString (Current_Coor.x,Current_Coor.y,buf);
}
void GUI_PrintfBack (const char *buf)
{
	GUI_PutStringBack (Current_Coor.x,Current_Coor.y,buf);
}
void GUI_PrintfBack_Rect (const char *buf)
{
	uint32_t x0,y0,x1,y1;
	uint8_t count = 0;
	while(buf[count]){
		count++;
	}
	x0 = Current_Coor.x;
	y0 = Current_Coor.y-2;
	x1 = Current_Coor.x + GUI_GetFontAscWith()*count+1;
	y1 = Current_Coor.y+2+GUI_GetFontAscHigh();
	GUI_RectangleFill(x0,y0,x1,y1,back_color);
	GUI_Rectangle(x0,y0,x1,y1,BLACK);
	Current_Coor.x++;
	GUI_PutStringBack (Current_Coor.x,Current_Coor.y,buf);
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/


