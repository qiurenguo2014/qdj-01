 /*************************************************************************
#    FileName: lcddrive.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-29 09:24:02
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __LCDDRIVE_H
#define __LCDDRIVE_H
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "../devdriver/ssd1906/ssd1906_hw_fb16b.h"
#define  TCOLOR uint16_t
#define  GUI_LCM_XMAX 320
#define  GUI_LCM_YMAX 240
/* Exported types ------------------------------------------------------*/
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
/* Exported variables --------------------------------------------------*/
extern volatile unsigned short *pSSD1906DAT;
/* Exported functions prototypes ---------------------------------------*/
extern void API_GUI_FillSCR(TCOLOR dat);
extern void API_GUI_ClearSCR(void);
extern void API_GUI_Point(int16_t x, int16_t y, TCOLOR color);
extern int API_GUI_ReadPoint(int16_t x, int16_t y, TCOLOR *ret);
extern void API_GUI_HLine(int16_t x0, int16_t y0, int16_t x1, TCOLOR color);
extern void API_GUI_RLine(int16_t x0, int16_t y0, int16_t y1, TCOLOR color);
extern void API_GUI_OpenBackLight (void);
extern void API_GUI_CloseBackLight (void);
extern void API_GUI_OpenSpeak (void);
extern void API_GUI_CloseSpeak (void);
extern void API_GUI_SetBackLightLevel (uint8_t level);
//extern int  GUI_CmpColor(TCOLOR color1, TCOLOR color2);
#define  GUI_CmpColor(color1, color2)	(color1==color2)
//extern void  GUI_CopyColor(TCOLOR *color1, TCOLOR color2);
#define  GUI_CopyColor(color1, color2) 	*color1 = color2
#endif 
/*********************************END OF FILE****************************/


