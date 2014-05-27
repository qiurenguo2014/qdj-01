/*************************************************************************
#    FileName: menu.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "lcddrive.h"
#include "gui_stockc.h"
#include "macro.h"
#include "loadbit.h"
#include "gui_basic.h"
#include "menu.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  选中效果.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_MenuDrawTitleFocus (const TITLE *title)
{
	GUI_SetAscType (title->asctype);
	GUI_SetHzkType (title->hzktype);
	GUI_SetColor(title->disp, 0xfd00);
	GUI_PutStringBack (title->coor.x,
			title->coor.y,
			title->name);
	return TRUE;
}

/**
  * @brief  没有选中效果.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_MenuDrawTitleFocus_no (const TITLE *title)
{
	GUI_SetAscType (title->asctype);
	GUI_SetHzkType (title->hzktype);
	GUI_SetColor(title->disp, title->back);
	GUI_PutStringBack (title->coor.x,
			title->coor.y,
			title->name);
	return TRUE;
}
uint8_t GUI_MenuDrawTitle (const TITLE *title)
{
	GUI_SetAscType (title->asctype);
	GUI_SetHzkType (title->hzktype);
	GUI_SetColor(title->disp, title->back);	
	GUI_PutStringBack (title->coor.x,
			title->coor.y,
			title->name);
	return TRUE;
}
/**
  * @brief  不显示隐藏菜单.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_MenuDrawShowFocus (const MENU *menu)
{
	const MENU *m = menu;
	uint8_t i;
	GUI_MenuDrawTitleFocus (m->title);
	m = m->next;
	for( i=0 ; i<200 ; i++ )
	{
		if(m==menu){
			break;
		}else{
			if(m->stat & MENU_STAT_HIDEMENU){
			}else{
				GUI_MenuDrawTitleFocus_no (m->title);
			}			
		}
		m = m->next;
	}
	return TRUE;
}
/**
  * @brief  显示隐藏菜单.
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t GUI_MenuDrawShowAllFocus (const MENU *menu)
{
	const MENU *m = menu;
	uint8_t i;
	GUI_MenuDrawTitleFocus (m->title);
	m = m->next;
	for( i=0 ; i<200 ; i++ )
	{
		if(m==menu){
			break;
		}else{
			GUI_MenuDrawTitleFocus_no (m->title);
		}
		m = m->next;
	}
	return TRUE;
}
/*
* @brief  取消选中效果.
* @param  None.
* @retval None.
* @verify .
*/
void GUI_MenuCancelFocus (const MENU *menu)
{
	GUI_MenuDrawTitleFocus_no (menu->title);
}
/*
* @brief  选中效果.
* @param  None.
* @retval None.
* @verify .
*/
void GUI_MenuEnterFocus (const MENU *menu)
{
	GUI_MenuDrawTitleFocus (menu->title);
}
uint8_t _MENU_Process_VK_UP (const MENU *menu)
{
	const MENU *m = menu,*mb;
	uint8_t ret = FALSE;
	uint8_t numa=0,numb;
	m = m->next;
	while(m->next != menu){
		m = m->next;
		numa ++;
	}
	if(m->stat&MENU_STAT_HIDEMENU){
		mb = m;
		for(numb=0; numb < numa; numb++){
			while(m->next != mb){
				m = m->next;
			}
			if(m->stat&MENU_STAT_HIDEMENU){
			}else{
				break;
			}
		}
	}
	GUI_MenuDrawShowFocus (m);
	ret = TRUE;
	return ret;
}
uint8_t _MENU_Process_VK_DOWN (const MENU *menu)
{
	const MENU *m = menu;
	uint8_t ret = FALSE;
	m = m->next;
	while(m->stat&MENU_STAT_HIDEMENU){
		m = m->next;
	}
	menu = m;
	GUI_MenuDrawShowFocus (m);
	ret = TRUE;
	return ret;
}
uint8_t _MENU_Process_VK_ENTER (const MENU *menu)
{
	const MENU *m = menu;
	uint8_t ret = FALSE;
	m = m->next;
	GUI_MenuDrawShowFocus (m);
	ret = TRUE;
	return ret;
}
uint8_t _MENU_Process_VK_BACK (const MENU *menu)
{
	const MENU *m = menu;
	uint8_t ret = FALSE;
	m = m->next;
	GUI_MenuDrawShowFocus (m);
	ret = TRUE;
	return ret;
}
uint8_t GUI_MenuProcessService (MENU_SERVICE_NO no,const MENU *p)
{
	switch(no){
		case MENU_VK_UP| VK_LOOSE:
			_MENU_Process_VK_UP (p);
			break;
		case MENU_VK_DOWN| VK_LOOSE:
			_MENU_Process_VK_DOWN (p);
			break;
		case MENU_VK_ENTER| VK_LOOSE:
			_MENU_Process_VK_ENTER (p);
			break;
		case MENU_VK_BACK| VK_LOOSE:
			_MENU_Process_VK_BACK (p);
			break;
		case MENU_VK_SYSSET| VK_LOOSE:
			break;
		default:
			break;
	}
	return TRUE;
}
void GUI_MenuCorrectPara (volatile int32_t *p,int16_t x, int16_t y,uint8_t pbit)
{
	uint8_t pb=0;
	uint8_t keymsg;
	while(1){
		switch(pb){
			case 0:
				sprintf(printf_buf, "%u", *p%10);
				break;
			case 1:
				sprintf(printf_buf, "%u", *p/10%10);
				break;
			case 2:
				sprintf(printf_buf, "%u", *p/100%10);
				break;
			case 3:
				sprintf(printf_buf, "%u", *p/1000l%10);
				break;
			case 4:
				sprintf(printf_buf, "%u", *p/10000l%10);
				break;
			case 5:
				sprintf(printf_buf, "%u", *p/100000l%10);
				break;
		}
		GUI_SetAscType (FONT_ASC_24);
		GUI_SetHzkType (FONT_HZK_24);
		GUI_PutStringBack(x+(pbit-pb-1)*12,y,printf_buf);
		keymsg = GUI_Get_Keymsg ();
		if(keymsg == (VK_ENTER| VK_LOOSE)){
			pb ++;
			if(pb >= pbit){
				break;
			}
		}else if(keymsg == (VK_BACK| VK_LOOSE)){
			if(pb == 0){
				break;
			}
			pb --;
		}else if(keymsg == (VK_DOWN)){
			if(pb==0){
				if((*p)%10>0){
					*p-=1;
				}else{
					*p+=9;
				}
			}else if(pb==1){
				if(*p/10%10>0){
					*p-=10;
				}else{
					*p+=90;
				}
			}else if(pb==2){
				if(*p/100%10>0){
					*p-=100;
				}else{
					*p+=900;
				}
			}else if(pb==3){
				if(*p/1000%10>0){
					*p-=1000;
				}else{
					*p+=9000;
				}
			}else if(pb==4){
				if(*p/10000%10>0){
					*p-=10000;
				}else{
					*p+=90000;
				}
			}
		}
	}
}

/*********************************END OF FILE****************************/



















