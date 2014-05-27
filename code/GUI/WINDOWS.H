/*************************************************************************
#    FileName: widget.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-10 09:27:25
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef  WINDOWS_H
#define  WINDOWS_H
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "gui_basic.h"
/* Exported types ------------------------------------------------------*/
typedef struct{
	uint32_t stat;
	PointXY point;
	TCOLOR disp;		//窗体显示颜色
	TCOLOR back;		//窗体边框显示颜色
}WINDOW;
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
uint8_t GUI_WindowDraw (const WINDOW *win);
#endif 
/*********************************END OF FILE****************************/

