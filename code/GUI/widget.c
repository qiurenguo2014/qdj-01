/*************************************************************************
#    FileName: widget.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-10 09:27:10
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include <stdio.h>
#include "../common/macro.h"
#include "gui_keyboard.h"
#include "widget.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
WIDGET *wm[WIDGET_MAX];
static WIDGET default_wm;
uint32_t GLOBAL_WIDGET_STAT;
volatile uint16_t focus_widget_no=0;
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  清除窗体.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WidgetClearPointerStruct (void)
{
	uint8_t ret = FALSE,i;
	focus_widget_no = 0;
	default_wm.focus_menu = 0;
	default_wm.menu = 0;
	default_wm.service = 0;
	default_wm.stat = 0;
	default_wm.win = 0;
	for(i=0; i<WIDGET_MAX; i++){
		wm[i] = &default_wm;
	}
	return ret;
}
/**
  * @brief  创建主窗体,编号为0.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WidgetCreatMain (WIDGET *p)
{
	uint8_t ret=FALSE;
	focus_widget_no = 0;
	wm[focus_widget_no] = p;
	GUI_WindowDraw (wm[focus_widget_no]->win);
	if(wm[focus_widget_no]->showbackground == NULL){
	}else{
		wm[focus_widget_no]->showbackground ();
	}
	if(wm[focus_widget_no]->stat & WIDGET_STAT_SHOWHIDE){
		GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
	}else{
		GUI_MenuDrawShowFocus (wm[focus_widget_no]->focus_menu);
	}
	ret = TRUE;
	return ret;
}
/**
  * @brief  更新指定窗体.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WidgetUpdataPoint (WIDGET *p)
{
	uint8_t ret = FALSE;
	GUI_WindowDraw (p->win);
	if(p->showbackground == NULL){
	}else{
		p->showbackground ();
	}
	if(p->stat & WIDGET_STAT_SHOWHIDE){
		GUI_MenuDrawShowAllFocus (p->focus_menu);
	}else{
		GUI_MenuDrawShowFocus (p->focus_menu);
	}
	ret = TRUE;
	return ret;
}
uint8_t GUI_WidgetUpdataFocus (void)
{
	return GUI_WidgetUpdataPoint (wm[focus_widget_no]);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WidgetUpdataAll (void)
{
	uint16_t widget_no;
	uint8_t ret = FALSE;
	API_GUI_ClearSCR ();
	for(widget_no=0; widget_no<=focus_widget_no; widget_no++){
		GUI_WidgetUpdataPoint (wm[widget_no]);
	}
	ret = TRUE;
	return ret;
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WidgetOpen (WIDGET *p)
{
	uint8_t ret=FALSE;
	focus_widget_no ++;
	/* 窗口打开最大量 */
	if (focus_widget_no >= WIDGET_MAX){
		focus_widget_no --;
	}else{
		wm[focus_widget_no] = p;
		wm[focus_widget_no]->focus_menu = &p->menu[0];
		/* 画窗体，设置偏移量和长宽 */
		GUI_WindowDraw (wm[focus_widget_no]->win);
		/* 检测是否有背景 */
		if(wm[focus_widget_no]->showbackground == NULL){
		}else{
			wm[focus_widget_no]->showbackground ();
		}
		/* 画菜单，清状态位 */
		wm[focus_widget_no]->stat = 0;
		GUI_MenuDrawShowFocus (wm[focus_widget_no]->focus_menu);
		if(p->service == NULL){
		}else{
			p->service (WIDGET_INIT_WIDGET,0);
		}
		ret = TRUE;
	}
	return ret;
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_WidgetClosePoint (WIDGET *p)
{
	uint8_t ret=FALSE;
	uint16_t widget_no;
	if (wm[0] == p){
	}else{
		for(widget_no=0; widget_no<WIDGET_MAX; widget_no++){
			if(wm[widget_no] == p){
				break;
			} 
		}
		if(widget_no > focus_widget_no){
		}else{			
			for(; widget_no<focus_widget_no; widget_no++){
				wm[widget_no] = wm[widget_no+1];
			}
			focus_widget_no--;
			GUI_WidgetUpdataAll ();
			ret = TRUE;
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
uint8_t GUI_WidgetCloseFocus (void)
{
	uint8_t ret=FALSE;
	if (focus_widget_no == 0){
	}else{
		focus_widget_no --;
		GUI_WidgetUpdataAll ();
		ret = TRUE;
	}
	return ret;
}
/*
* @brief  跳转下一个菜单.
* @param  None.
* @retval None.
* @verify .
*/
void GUI_WidgetJumpNextMenu (void)
{
	const MENU *m = wm[focus_widget_no]->focus_menu;
	GUI_MenuCancelFocus (m);
	m = m->next;
	if(wm[focus_widget_no]->stat & WIDGET_STAT_SHOWHIDE){
		//m = m->next;
	}else{
		while(m->stat&MENU_STAT_HIDEMENU){
			m = m->next;
		}
	}
	wm[focus_widget_no]->focus_menu = m;
	GUI_MenuEnterFocus (m);
}
/*
* @brief  跳转下一级菜单.
* @param  None.
* @retval None.
* @verify .
*/
void GUI_WidgetJumpNextLevelMenu (void)
{
	uint16_t i;
	const MENU *p = wm[focus_widget_no]->focus_menu->next_level;
	if(p ==NULL){
	}else{
		/* 查找对应窗体 */
		for(i=0; i<WIDGET_MAX; i++){
			if(widget[i].menu == p){//找到对应窗体
				GUI_WidgetOpen (&widget[i]);
				break;
			}
		}
	}
}
void _WIDGET_Process_VK_UP_LOOSE (void)
{
}
void _WIDGET_Process_VK_DOWN_LOOSE (void)
{
	GUI_WidgetJumpNextMenu ();
}
void _WIDGET_Process_VK_ENTER_LOOSE (void)
{
	GUI_WidgetJumpNextLevelMenu ();
}
uint8_t GUI_WidgetDirectProcessService (WIDGET_SERVICE_NO no,WIDGET *p)
{
	uint8_t keymsg,ret=FALSE;
	switch(no){
		case WIDGET_VK_UP:
			break;
		case WIDGET_VK_DOWN:
			_WIDGET_Process_VK_DOWN_LOOSE ();
			break;
		case WIDGET_VK_ENTER:
			_WIDGET_Process_VK_ENTER_LOOSE ();
			break;
		case WIDGET_VK_BACK:
			GUI_WidgetCloseFocus ();
			break;
		case WIDGET_VK_SYSSET:
			wm[focus_widget_no]->stat |= WIDGET_STAT_SHOWHIDE;
			GUI_WidgetUpdataFocus ();
			break;
		case WIDGET_VK_UP| WIDGET_VK_LOOSE:
			break;
		case WIDGET_VK_DOWN| WIDGET_VK_LOOSE:
			
			break;
		case WIDGET_VK_ENTER| WIDGET_VK_LOOSE:
			
			break;
		case WIDGET_VK_BACK| WIDGET_VK_LOOSE:
			
			break;
		case WIDGET_VK_SYSSET| WIDGET_VK_LOOSE:
			break;
		case WIDGET_CREAT_MAINWIDGE:
			GUI_WidgetCreatMain (p);
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetUpdataFocus ();
			break;
		case WIDGET_CLOSE_POINTWIDGET:
			GUI_WidgetClosePoint (p);
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			GUI_WidgetClosePoint (wm[focus_widget_no]);
			break;
		case WIDGET_OPEN_WIDGET:
			GUI_WidgetOpen (p);
			break;
		case WIDGET_PROCESS_KEYMSG:			
			if(wm[focus_widget_no]->focus_menu->service == NULL){
				keymsg = GUI_Get_Keymsg ();
				if(keymsg == VK_NOKEY){
				}else{
					GUI_WidgetDirectProcessService (keymsg,0);
				}
			}else{
				wm[focus_widget_no]->focus_menu->service(WIDGET_PROCESS_KEYMSG,0);
			}
			
			break;
		default:
			break;
	}
	ret = TRUE;
	return ret;
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */

uint8_t GUI_WidgetProcessService (WIDGET_SERVICE_NO no,WIDGET *p)
{
	uint8_t ret = FALSE;

	if(wm[focus_widget_no]->service == NULL){
		GUI_WidgetDirectProcessService (no,p);
	}else{
		wm[focus_widget_no]->service(no,p);
	}
	ret = TRUE;
	return ret;
}

/*********************************END OF FILE****************************/


