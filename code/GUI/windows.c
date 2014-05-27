/*************************************************************************
#    FileName: windows.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "lcddrive.h"
#include "gui_basic.h"
#include "gui_stockc.h"
#include "macro.h"
#include "windows.h"
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
uint8_t GUI_WindowDraw (const WINDOW *win)
{
	GUI_Set_X0_Y0_Base (&win->point);
	GUI_Rectangle (0, 0,win->point.with,
			win->point.hight,win->back);
	GUI_RectangleFill (1, 1,
			win->point.with-1,
			win->point.hight-1,
			win->disp);
	return TRUE;
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WindowsClr(const WINDOW *win)
{
	GUI_Set_X0_Y0_Base (&win->point);
	GUI_Rectangle (0, 0,win->point.with,
			win->point.hight,win->back);
	GUI_RectangleFill (1, 1,
			win->point.with-1,
			win->point.hight-1,
			win->disp);
	return TRUE;
}


