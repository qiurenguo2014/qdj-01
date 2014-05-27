/*************************************************************************
#    FileName: widget.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-10 09:27:25
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef  MENU_H
#define  MENU_H
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "windows.h"
#include "gui_font.h"
#include "gui_keyboard.h"
/* Exported types ------------------------------------------------------*/
struct MENU_STRUCT;
typedef enum{
	MENU_VK_LOOSE = VK_LOOSE,
	MENU_VK_UP = VK_UP,
	MENU_VK_DOWN = VK_DOWN,
	MENU_VK_ENTER = VK_ENTER,
	MENU_VK_BACK = VK_BACK,
	MENU_VK_SYSSET = VK_SYSSET,	
	MENU_PROCESS_KEYMSG = 0x100,
}MENU_SERVICE_NO;
typedef void (*FunctionM) (MENU_SERVICE_NO service_no,
			const struct MENU_STRUCT *p);
typedef struct MENU_TITLE{
	PointXY coor;
	char *name;
	FONT_HZK_TYPE hzktype;
	FONT_ASC_TYPE asctype;
	TCOLOR disp;
	TCOLOR back;
}TITLE;
typedef struct MENU_STRUCT{
	uint32_t stat;
	const TITLE *title;
	FunctionM service;
	const struct MENU_STRUCT *next;//指向下一个菜单，必须是一个循环
	const struct MENU_STRUCT *next_level;//下级菜单
	const struct MENU_STRUCT *head;
	const struct MENU_STRUCT *head_level;	
}MENU;
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
#define MENU_STAT_HIDEMENU (1<<0)
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern uint8_t GUI_MenuPorcessService (MENU_SERVICE_NO no,
					const struct MENU_STRUCT *p);
extern uint8_t GUI_MenuDrawTitleFocus (const TITLE *title);
extern uint8_t GUI_MenuDrawTitleFocus_no (const TITLE *title);
extern uint8_t GUI_MenuDrawTitle (const TITLE *title);
extern uint8_t GUI_MenuDrawShowFocus (const MENU *menu);
extern uint8_t GUI_MenuDrawShowAllFocus (const MENU *menu);
extern void GUI_MenuCancelFocus (const MENU *menu);
extern void GUI_MenuEnterFocus (const MENU *menu);
#endif

/*********************************END OF FILE****************************/


