/*************************************************************************
#    FileName: widget.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-10 09:27:25
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __WIDGET_H
#define __WIDGET_H

/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "windows.h"
#include "menu.h"
/* Exported types ------------------------------------------------------*/
typedef enum{
	WIDGET_VK_LOOSE = VK_LOOSE,
	WIDGET_VK_UP = VK_UP,
	WIDGET_VK_DOWN = VK_DOWN,
	WIDGET_VK_ENTER = VK_ENTER,
	WIDGET_VK_BACK = VK_BACK,
	WIDGET_VK_SYSSET = VK_SYSSET,
	WIDGET_CREAT_MAINWIDGE = 0x100,	//创建主窗体
	WIDGET_INIT_WIDGET,			//窗体初始化
	WIDGET_EXIT_WIDGET,			//窗体退出调用
	WIDGET_UPDATA_ALLWIDGET,
	WIDGET_UPDATA_FOCUSWIDGET,
	WIDGET_CLOSE_WIDGET,		//
	WIDGET_CLOSE_POINTWIDGET,	//关闭指定窗体
	WIDGET_CLOSE_FOCUSWIDGET,	//关闭当前窗体
	WIDGET_OPEN_WIDGET,			//开启窗体
	WIDGET_UPDATA_THISWIDGET,	//刷新自己的窗体
	WIDGET_PROCESS_KEYMSG,		//处理按键信息
	WIDGET_UPDATA_DATA,
}WIDGET_SERVICE_NO;
/* 定义窗口控件结构体  */
struct WIDGET_STRUCT;
typedef uint8_t (*FunctionW) (WIDGET_SERVICE_NO service_no,struct WIDGET_STRUCT *p);
typedef void (*Function0) (void);
typedef struct WIDGET_STRUCT{
	uint32_t stat;		//状态
	const MENU *focus_menu;
	const WINDOW *win;//窗口属性,大小,位置
	const MENU *menu;//
	FunctionW service;//为空时，执行默认服务函数
	Function0 showbackground;//显示背景
}WIDGET;
/* Exported constants --------------------------------------------------*/
#define WIDGET_MAX 20
extern WIDGET *wm[WIDGET_MAX];
extern volatile uint16_t focus_widget_no;
extern WIDGET widget[WIDGET_MAX];
/* Exported define -----------------------------------------------------*/
#define WIDGET_STAT_SHOWHIDE (1<<0)
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern uint8_t GUI_WidgetClearPointerStruct (void);
extern uint8_t GUI_WidgetPorcessService (WIDGET_SERVICE_NO no,const struct WIDGET_STRUCT *p);
extern uint8_t GUI_WidgetProcessService (WIDGET_SERVICE_NO no,WIDGET *p);
extern uint8_t GUI_WidgetUpdataPoint (WIDGET *p);
extern uint8_t GUI_WidgetUpdataFocus (void);
extern uint8_t GUI_WidgetUpdataAll (void);
extern uint8_t GUI_WidgetOpen (WIDGET *p);
extern uint8_t GUI_WidgetClosePoint (WIDGET *p);
extern uint8_t GUI_WidgetCloseFocus (void);
extern void GUI_WidgetJumpNextMenu (void);
extern void GUI_WidgetJumpNextLevelMenu (void);
extern uint8_t GUI_WidgetDirectProcessService (WIDGET_SERVICE_NO no,WIDGET *p);
#endif 
/*********************************END OF FILE****************************/


