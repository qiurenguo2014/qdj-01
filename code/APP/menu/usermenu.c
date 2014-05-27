/*************************************************************************
#    FileName: usermenu.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-10 16:45:44
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include <stdio.h>
#include "../../common/macro.h"
#include "../signal/sig_process.h"
#include "../../gui/windows.h"
#include "../../gui/widget.h"
#include "../../gui/menu.h"
#include "../../gui/gui_keyboard.h"
#include "../../gui/gui_font.h"
#include "../../gui/lcddrive.h"
#include "../../gui/gui_stockc.h"
#include "../../devdriver/ssd1906/ssd1906_hw_fb16b.h"
#include "usermenu.h"
/* Types ---------------------------------------------------------------*/
//extern const unsigned char ImagBackGround[153600];
void _UM_ShowBackGround (void);
WIDGET *current_widget;
#define DEFAULT_BACK 0xB6FE
const TITLE value[]={
{64,41,0,0,  "母线  电压:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{64,63,0,0,  "正对地电阻:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{64,85,0,0,  "负对地电阻:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{64,131,0,0, "对地  电容:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{64,153,0,0, "系统  容抗:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{64,175,0,0, "系统  阻抗:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//5
{64,41,0,0,  "系统  容抗:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//6
{64,63,0,0,  "系统  阻抗:    ",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//7
{8,0,0,0,    "当前状态:",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//8
};
const TITLE title[]={
/* x|y|w|h|*title|disp|back */
{88+48,0+28,0,0, "电阻测量",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},
{88+48,32+28,0,0,"阻抗测量",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},
{88+48,64+28,0,0,"环网查找",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},
{88+48,96+28,0,0,"系统设置",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},//3
{112,250,0,0,"启动测量",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},//4
{112+24,250,0,0,"查找环网",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},//5

{8,0,0,0,  "零 偏  修 正",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//6
{8,16,0,0, "母线 正 电压:",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{8,32,0,0, "母线 负 电压:",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{8,48,0,0, "R 正对地系数:",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},
{8,64,0,0, "R 负对地系数:",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//10
{8,80,0,0,"电容修正系数:",FONT_HZK_16,FONT_ASC_16,BLACK,DEFAULT_BACK},//11
{8,144,0,0,"重置修正系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//12
{88+48,96+88,0,0,"修正界面",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},
{8,24,0,0,"时间设置",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//14
{8,72,0,0,"屏幕亮度",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//15
{8,48,0,0,"背光时间",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//16
{8,80,0,0,"容抗修正系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//17
{8,80,0,0,"阻抗修正系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//18

{124,24,0,0, "年",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//19
{196,24,0,0,"月",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//
{268,24,0,0, "日",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//
{124,56,0,0,"时",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//
{196,56,0,0, "分",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//
{268,56,0,0, "秒",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//
{40,108,0,0,"屏幕亮度:",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//
{40,138,0,0,"背光时间:",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//26
{88+48,128+28,0,0,"关闭设备",FONT_HZK_24,FONT_ASC_24,BLACK,DEFAULT_BACK},//27
{40,168,0,0,"蜂 鸣 器:",FONT_HZK_24, FONT_ASC_24, BLACK, DEFAULT_BACK},//28
{8,128+16,0,0,"电容零偏系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//29

{8,96,0,0,"电容1x系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//30
{8,112,0,0,"电容2x系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//31
{8,128,0,0,"电容3x系数",FONT_HZK_16, FONT_ASC_16,BLACK,DEFAULT_BACK},//32

};
/* 对地电阻测量菜单 */
const MENU menu1[] ={
/* stat|*title|service|*next|nextlevel|head|headlevel */
{0,&title[4],0,&menu1[0],0,&menu1[0],0},
};
/* 对地阻抗测量菜单 */
const MENU menu2[] ={
/* stat|*title|service|*next|nextlevel|head|headlevel */
{0,&title[4],0,&menu2[0],0,&menu2[0],0},
};
/* 环网查找菜单 */
const MENU menu3[] ={
{0, &title[5],0,&menu3[0],0,&menu3[0],0},
};
/* 系统设置菜单 */
void Menu_ServiceTimeProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceYearProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceMonProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceDayProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceHourProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceMinProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceSecProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceBeepProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceLightLevelProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceLightTimeProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
const MENU menu5[]={
{0,&title[19],Menu_ServiceYearProcess,&menu5[1],0,&menu5[8],0},
{0,&title[20],Menu_ServiceMonProcess,&menu5[2],0,&menu5[0],0},
{0,&title[21],Menu_ServiceDayProcess,&menu5[3],0,&menu5[1],0},
{0,&title[22],Menu_ServiceHourProcess,&menu5[4],0,&menu5[2],0},
{0,&title[23],Menu_ServiceMinProcess,&menu5[5],0,&menu5[3],0},
{0,&title[24],Menu_ServiceSecProcess,&menu5[6],0,&menu5[4],0},
{0,&title[25],Menu_ServiceLightLevelProcess,&menu5[7],0,&menu5[5],0},
{0,&title[26],Menu_ServiceLightTimeProcess,&menu5[8],0,&menu5[6],0},
{0,&title[28],Menu_ServiceBeepProcess,&menu5[0],0,&menu5[7],0},
};
/* 修正菜单 */
void Menu_ServiceZeroProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceGEZProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceGEFProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceRZProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceRFProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceCPProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceCRProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceRRProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceCAP1XProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceCAP2XProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceCAP3XProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
void Menu_ServiceRESETProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
const MENU menu4[] ={
/* 零偏 */
{0, &title[6],&Menu_ServiceZeroProcess,&menu4[1],0,&menu4[10],0},
/* 母线正 */
{0, &title[7],&Menu_ServiceGEZProcess,&menu4[2],0,&menu4[0],0},
/* 母线负 */
{0, &title[8],&Menu_ServiceGEFProcess,&menu4[3],0,&menu4[1],0},
/* 正对地电阻 */
{0, &title[9],&Menu_ServiceRZProcess,&menu4[4],0,&menu4[2],0},
/* 负对地电阻 */
{0, &title[10],&Menu_ServiceRFProcess,&menu4[5],0,&menu4[3],0},
/* 容抗修正 */
//{0, &title[17],&Menu_ServiceCRProcess,&menu4[6],0,&menu4[4],0},
/* 阻抗修正 */
{0, &title[18],&Menu_ServiceRRProcess,&menu4[6],0,&menu4[4],0},
/* 电容1x修正 */
{0, &title[30],&Menu_ServiceCAP1XProcess,&menu4[7],0,&menu4[5],0},
/* 电容2x修正 */
{0, &title[31],&Menu_ServiceCAP2XProcess,&menu4[8],0,&menu4[6],0},
/* 电容3x修正 */
{0, &title[32],&Menu_ServiceCAP3XProcess,&menu4[9],0,&menu4[7],0},
/* 重置修正 */
{0, &title[12],&Menu_ServiceRESETProcess,&menu4[0],0,&menu4[8],0},
};
/* 主菜单 */
void Menu_ServicePOWERProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p);
const MENU menu[] ={
/* stat|*title|service|*next|nextlevel|head|headlevel */
{0,&title[0],0,&menu[1],menu1,&menu[5],0},
{0,&title[1],0,&menu[2],menu2,&menu[0],0},
{0,&title[2],0,&menu[3],menu3,&menu[1],0},
{0,&title[3],0,&menu[4],menu5,&menu[2],0},
{0,&title[27],&Menu_ServicePOWERProcess,&menu[5],0,&menu[3],0},
{MENU_STAT_HIDEMENU,&title[13],0,&menu[0],menu4,&menu[4],0},
};
const WINDOW win[] ={
{0,0,17,320-1,240-17-1,LBBLUE,LBBLUE},
{0,0,0,320-1,16,LGRAYBLUE,LGRAYBLUE},
};
/*  */
uint8_t _UM_EarthResProcessService (WIDGET_SERVICE_NO no,WIDGET *p);
uint8_t _UM_EarthCapProcessService (WIDGET_SERVICE_NO no,WIDGET *p);
uint8_t _UM_EarthSearchProcessService (WIDGET_SERVICE_NO no, WIDGET *p);
uint8_t _UM_EarthEorProcessService (WIDGET_SERVICE_NO no,WIDGET *p);
uint8_t _UM_SystemSetProcessService (WIDGET_SERVICE_NO no,WIDGET *p);
void _UM_ShowMainWidget_Background (void);
void _UM_ShowDefault_Background(void);
WIDGET widget[WIDGET_MAX] ={
/* stat|focus menu no|*win |*menu |sevice|background*/
{0,&menu[0],&win[0],&menu[0],0,
								&_UM_ShowMainWidget_Background},
{0,&menu1[0],&win[0],&menu1[0],&_UM_EarthResProcessService,
								&_UM_ShowDefault_Background},
{0,&menu2[0],&win[0],&menu2[0],&_UM_EarthCapProcessService,
								&_UM_ShowDefault_Background},
{0,&menu3[0],&win[0],&menu3[0],&_UM_EarthSearchProcessService,
								&_UM_ShowDefault_Background},
{0,&menu4[0],&win[0],&menu4[0],&_UM_EarthEorProcessService,
								&_UM_ShowDefault_Background},
{0,&menu5[0],&win[0],&menu5[0],&_UM_SystemSetProcessService,
								&_UM_ShowDefault_Background},
};
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* 从左边数起来按键 */
#define COOR_K1_X 24
#define COOR_K1_Y 207
#define COOR_K2_X 144
#define COOR_K2_Y 207
#define COOR_K3_X 264
#define COOR_K3_Y 207
/* 下边栏的颜色 */
#define UNDER_COLU_COLOR 0X865D
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void UM_Configuration (void)
{
	API_GUI_ClearSCR ();
	//API_GUI_OpenBackLight ();
	//API_GUI_SetBackLightLevel (UM_GET_SYSTEMPARA&PARA_LIGHT_MASK);
	API_GUI_SetBackLightLevel (2);
	GUI_Font_Init ();
	GUI_WidgetClearPointerStruct (); 
	GUI_WidgetProcessService (WIDGET_CREAT_MAINWIDGE,&widget[0]);
}
/* 电源控制 */
static void _UM_POWERProcessKey (uint8_t keymsg)
{
	uint16_t x;
	uint16_t delay=0;
	switch(keymsg){
		case VK_DOWN:
			GUI_WidgetJumpNextMenu();
			break;
		case VK_BACK:
			break;
		case VK_ENTER:
			if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
				API_GUI_OpenSpeak ();
			}
			for(x=0; x<320; x++){
				GUI_RLine(x, 0, 240,BLACK);
				//while(delay--);
			}
			API_GUI_SetBackLightLevel (0);
			printf("\r\npower down...");
			API_GUI_CloseSpeak ();
			BSP_PowerDown ();
			while(1);
		default:
			break;
	}
}
void Menu_ServicePOWERProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case WIDGET_INIT_WIDGET:
			break;
		case WIDGET_EXIT_WIDGET:
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetUpdataFocus ();
			break;
		case WIDGET_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			if(keymsg == VK_NOKEY){
			}else{
				_UM_POWERProcessKey (keymsg);
			}
			break;
		default:
			break;
	}
}
void _UM_MEASURE_STATUS (void)
{
	GUI_MenuDrawTitle (&value[8]);
	GUI_SetColor (BLUE, DEFAULT_BACK);
	if(SIGPRC_Struct.status & SIG_STAT_READY){
		GUI_PrintfBack ("准备就绪");
	}else if(SIGPRC_Struct.status & SIG_STAT_MEASURING){
		GUI_PrintfBack ("正在测量");
	}else if(SIGPRC_Struct.status & SIG_STAT_MEASUREOK){
		GUI_PrintfBack ("测量完成");
	}else if(SIGPRC_Struct.status & SIG_STAT_CANCEL){
		GUI_PrintfBack ("取消测量");
	}
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_PrintfBack ("(请检查黄色地线!!!)");
	}else{
		GUI_PrintfBack ("                     ");
	}
}
/* 显示电阻测量的变量 */
void _UM_EarthResShowBack (void)
{
	uint16_t x,y;
	_UM_MEASURE_STATUS ();
	GUI_MenuDrawTitle (&value[0]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	sprintf(printf_buf,"     伏特");
	GUI_PrintfBack_Rect (printf_buf);

	GUI_MenuDrawTitle (&value[1]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	sprintf(printf_buf,"     千欧");
	GUI_PrintfBack_Rect (printf_buf);

	GUI_MenuDrawTitle (&value[2]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	sprintf(printf_buf,"     千欧");
	GUI_PrintfBack_Rect (printf_buf);
	for(y=224; y<240; y++){
		for(x=0; x<320; x++){
			API_GUI_Point (x, y, UNDER_COLU_COLOR);
		}
	}
	GUI_PutString(COOR_K2_X,COOR_K2_Y,"退出");
	GUI_PutString(COOR_K3_X,COOR_K3_Y,"测量");
	GUI_Rectangle (8,32,312,110,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_PutStringBack(20,24," 电阻测量 ");
}
void _UM_EarthResShowValue (void)
{
	uint16_t x,y;
	_UM_MEASURE_STATUS ();
	GUI_MenuDrawTitle (&value[0]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.GE_VOL > 999){
		SIGPRC_Struct.GE_VOL = 1000;
		GUI_PrintfBack_Rect (" >999伏特");
	}else if(SIGPRC_Struct.GE_VOL < 1){
		sprintf(printf_buf,"   <1伏特");
		GUI_PrintfBack_Rect (printf_buf);
	}else{
		sprintf(printf_buf,"  %3d伏特",(int32_t)SIGPRC_Struct.GE_VOL);
		GUI_PrintfBack_Rect (printf_buf);
	}
	GUI_MenuDrawTitle (&value[1]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.ZR_RES > 999){
		SIGPRC_Struct.ZR_RES = 1000;
		GUI_PrintfBack_Rect (" >999千欧");
	}else if(SIGPRC_Struct.ZR_RES < 1){
		sprintf(printf_buf,"   <1千欧");
		GUI_PrintfBack_Rect (printf_buf);
	}else{
		sprintf(printf_buf,"  %3d千欧",(int32_t)SIGPRC_Struct.ZR_RES);
		GUI_PrintfBack_Rect (printf_buf);
	}
	GUI_MenuDrawTitle (&value[2]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.FR_RES > 999){
		SIGPRC_Struct.FR_RES = 1000;
		GUI_PrintfBack_Rect (" >999千欧");
	}else if(SIGPRC_Struct.FR_RES < 1){
		sprintf(printf_buf,"   <1千欧");
		GUI_PrintfBack_Rect (printf_buf);
	}else{
		sprintf(printf_buf,"  %3d千欧",(int32_t)SIGPRC_Struct.FR_RES);
		GUI_PrintfBack_Rect (printf_buf);
	}
	for(y=224; y<240; y++){
		for(x=0; x<320; x++){
			API_GUI_Point (x, y, UNDER_COLU_COLOR);
		}
	}
	GUI_SetColor (BLACK, LIGHTGRAY);
	GUI_PutString(COOR_K2_X,COOR_K2_Y,"退出");
	GUI_PutString(COOR_K3_X,COOR_K3_Y,"测量");
	GUI_Rectangle (8,32,312,110,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_PutStringBack(20,24," 电阻测量 ");
}
static void _UM_EarthResProcessInit (void)
{
	SIGPRC_Struct.status |= SIG_STAT_READY;
	SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
								SIG_STAT_MEASURING |
								SIG_STAT_EARTHLINE |
								SIG_STAT_CANCEL);
	SIGPRC_Struct.GE_VOL = 0;
	SIGPRC_Struct.ZR_RES = 0;
	SIGPRC_Struct.FR_RES = 0;
	_UM_EarthResShowBack ();
}
static void _UM_EarthResProcessExit (void)
{
	SIGPRC_Struct.GE_VOL = 0;
	SIGPRC_Struct.ZR_RES = 0;
	SIGPRC_Struct.FR_RES = 0;
	SIG_StopConverEarthResCollect();
}
static void _UM_EarthResProcessKey (uint8_t keymsg)
{
	switch(keymsg){
		case VK_BACK:
			if(SIGPRC_Struct.status & SIG_STAT_ERCLG){
				SIG_StopConverEarthResCollect();
				SIGPRC_Struct.status |= SIG_STAT_CANCEL;
				SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
											SIG_STAT_MEASURING |
											SIG_STAT_EARTHLINE |
											SIG_STAT_READY);
				GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET, 0);
				_UM_EarthResShowBack ();
			}else{
				GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET, 0);
			}
			break;
		case VK_ENTER:
			if(SIGPRC_Struct.status & SIG_STAT_ERCLG){
			}else{
				_UM_EarthResProcessInit();
				SIGPRC_Struct.status &= ~SIG_STAT_EARTHLINE;
				SIGPRC_Struct.status &= ~SIG_STAT_READY;
				SIGPRC_Struct.status &= ~SIG_STAT_MEASUREOK;
				SIGPRC_Struct.status &= ~SIG_STAT_CANCEL;
				SIGPRC_Struct.status |= SIG_STAT_MEASURING;
				_UM_MEASURE_STATUS ();
				SIG_StartConverEarthResCollect ();
			}
			break;
		default:
			break;
	}
}
uint8_t _UM_EarthResProcessService (WIDGET_SERVICE_NO no,WIDGET *p)
{
	uint8_t keymsg,ret=FALSE;
	switch(no){
		case WIDGET_INIT_WIDGET:
			_UM_EarthResProcessInit ();
			break;
		case WIDGET_EXIT_WIDGET:
			_UM_EarthResProcessExit ();
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			_UM_EarthResProcessExit ();
			GUI_WidgetCloseFocus ();
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetUpdataFocus ();
			_UM_EarthResShowValue ();
			break;
		case WIDGET_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			if(keymsg == VK_NOKEY){
			}else{
				_UM_EarthResProcessKey (keymsg);
			}
			break;
		default:
			break;
	}
	ret = TRUE;
	return ret;
}
/* 显示背景，不显示数字 */
void _UM_EarthCapShowBack (void)
{
	_UM_EarthResShowBack ();
	GUI_MenuDrawTitle (&value[3]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	GUI_PrintfBack_Rect ("     微法");
	
	GUI_MenuDrawTitle (&value[4]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	GUI_PrintfBack_Rect ("     千欧");
	
	GUI_MenuDrawTitle (&value[5]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	GUI_PrintfBack_Rect ("     千欧");
	GUI_Rectangle (8,114+8,312,199,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_PutStringBack(20,114," 容抗测量 ");
}
/* 显示背景加显示数字 */
void _UM_EarthCapShowValue (void)
{
	_UM_EarthResShowValue ();
	GUI_MenuDrawTitle (&value[3]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.CP_CAP > 999){
		SIGPRC_Struct.CP_CAP = 1000;
		GUI_PrintfBack_Rect (" >999微法");
	}else if(SIGPRC_Struct.CP_CAP < 1){
		sprintf(printf_buf,"   <1微法");
		GUI_PrintfBack_Rect (printf_buf);
	}else{
		sprintf(printf_buf,"  %3d微法",(int32_t)SIGPRC_Struct.CP_CAP);
		GUI_PrintfBack_Rect (printf_buf);
	}
	GUI_MenuDrawTitle (&value[4]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.CR_SYS > 999){
		SIGPRC_Struct.CR_SYS = 1000;
		GUI_PrintfBack_Rect (" >999千欧");
	}else if(SIGPRC_Struct.CR_SYS < 1){
		sprintf(printf_buf,"   <1千欧");
		GUI_PrintfBack_Rect (printf_buf);
	}else{
		sprintf(printf_buf,"  %3d千欧",(int32_t)SIGPRC_Struct.CR_SYS);
		GUI_PrintfBack_Rect (printf_buf);
	}
	
	GUI_MenuDrawTitle (&value[5]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.RR_SYS > 999){
		SIGPRC_Struct.RR_SYS = 1000;
		GUI_PrintfBack_Rect (" >999千欧");
	}else if(SIGPRC_Struct.RR_SYS < 1){
		sprintf(printf_buf,"   <1千欧");
		GUI_PrintfBack_Rect (printf_buf);
	}else{
		sprintf(printf_buf,"  %3d千欧",(int32_t)SIGPRC_Struct.RR_SYS);
		GUI_PrintfBack_Rect (printf_buf);
	}
	GUI_Rectangle (8,114+8,312,199,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_PutStringBack(20,114," 容抗测量 ");
}
static void _UM_EarthCapProcessInit (void)
{
	SIGPRC_Struct.status |= SIG_STAT_READY;
	SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
								SIG_STAT_MEASURING |
								SIG_STAT_EARTHLINE |
								SIG_STAT_CANCEL);
	SIGPRC_Struct.GE_VOL = 0;
	SIGPRC_Struct.ZR_RES = 0;
	SIGPRC_Struct.FR_RES = 0;
	SIGPRC_Struct.CP_CAP = 0;
	SIGPRC_Struct.RR_SYS = 0;
	SIGPRC_Struct.CR_SYS = 0;
	_UM_EarthCapShowBack ();
}
static void _UM_EarthCapProcessExit (void)
{
	SIGPRC_Struct.GE_VOL = 0;
	SIGPRC_Struct.ZR_RES = 0;
	SIGPRC_Struct.FR_RES = 0;
	SIGPRC_Struct.CP_CAP = 0;
	SIGPRC_Struct.RR_SYS = 0;
	SIGPRC_Struct.CR_SYS = 0;
	SIG_StopConverEarthCapCollect();
}
static void _UM_EarthCapProcessKey (uint8_t keymsg)
{
	switch(keymsg){
		case VK_BACK:
			if(SIGPRC_Struct.status & SIG_STAT_ECCLG){
				SIG_StopConverEarthCapCollect();
				SIGPRC_Struct.status |= SIG_STAT_CANCEL;
				SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
											SIG_STAT_MEASURING |
											SIG_STAT_EARTHLINE |
											SIG_STAT_READY);
				GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
				_UM_EarthCapShowBack();
			}else{
				GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET, 0);
			}
			break;
		case VK_BACK|VK_LOOSE:
			if(SIGPRC_Struct.status & SIG_STAT_ECCLG){
				SIG_StopConverEarthCapCollect();
				SIGPRC_Struct.status |= SIG_STAT_CANCEL;
				SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
											SIG_STAT_MEASURING |
											SIG_STAT_EARTHLINE |
											SIG_STAT_READY);
				GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
				_UM_EarthCapShowBack();
			}
			break;
		case VK_ENTER:
			if(SIGPRC_Struct.status & SIG_STAT_ECCLG){
			}else{
				_UM_EarthCapProcessInit();
				SIGPRC_Struct.status &= ~SIG_STAT_READY;
				SIGPRC_Struct.status &= ~SIG_STAT_MEASUREOK;
				SIGPRC_Struct.status &= ~SIG_STAT_CANCEL;
				SIGPRC_Struct.status |= SIG_STAT_MEASURING;
				_UM_MEASURE_STATUS ();
				SIG_StartConverEarthCapCollect ();
			}
			break;
	}
}
uint8_t _UM_EarthCapProcessService (WIDGET_SERVICE_NO no,WIDGET *p)
{
	uint8_t keymsg,ret=FALSE;
	switch(no){
		case WIDGET_INIT_WIDGET:
			_UM_EarthCapProcessInit ();
			break;
		case WIDGET_EXIT_WIDGET:
			_UM_EarthCapProcessExit ();
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			_UM_EarthCapProcessExit ();
			GUI_WidgetCloseFocus ();
			break;
		case WIDGET_OPEN_WIDGET:
			//GUI_WidgetDirectProcessService (WIDGET_OPEN_WIDGET, p);
			//_UM_EarthCapProcessInit ();
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetDirectProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
			_UM_EarthCapShowValue ();
			break;
		case WIDGET_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			if(keymsg == VK_NOKEY){
			}else{
				_UM_EarthCapProcessKey (keymsg);
			}
			break;
		default:
			break;
	}
	ret = TRUE;
	return ret;
}
void _UM_EarthSearchStatus (void)
{
	GUI_MenuDrawTitle (&value[8]);
	GUI_SetColor (BLUE, DEFAULT_BACK);
	if(SIGPRC_Struct.status & SIG_STAT_READY){
		GUI_PrintfBack ("准备就绪");
	}else if(SIGPRC_Struct.status & SIG_STAT_MEASURING){
		GUI_PrintfBack ("正在测量");
	}else if(SIGPRC_Struct.status & SIG_STAT_MEASUREOK){
		GUI_PrintfBack ("测量完成");
	}else if(SIGPRC_Struct.status & SIG_STAT_CANCEL){
		GUI_PrintfBack ("取消测量");
	}
	if(SIGPRC_Struct.status & SIG_STAT_ERSCH_COM){
		GUI_MenuDrawTitle (&value[8]);
		GUI_SetColor (BLUE, DEFAULT_BACK);
		GUI_PrintfBack ("正在通讯");
	}
	if(SIGPRC_Struct.status & SIG_STAT_ERSCH_SERCHING){
		GUI_MenuDrawTitle (&value[8]);
		GUI_SetColor (BLUE, DEFAULT_BACK);
		GUI_PrintfBack ("正在查找");
	}
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_PrintfBack ("(请检查黄色地线!!!)");
	}else{
		GUI_PrintfBack ("                     ");
	}
}
/* 显示背景，不显示数字 */
void _UM_EarthSearchShowBack (void)
{
	uint16_t x,y;
	GUI_MenuDrawTitle (&value[6]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	GUI_PrintfBack_Rect ("     千欧");
	
	GUI_MenuDrawTitle (&value[7]);
	GUI_SetColor (BLACK, LIGHTGRAY);
	GUI_PrintfBack_Rect ("     千欧");
	for(y=224; y<240; y++){
		for(x=0; x<320; x++){
			API_GUI_Point (x, y, UNDER_COLU_COLOR);
		}
	}
	GUI_PutString(COOR_K2_X,COOR_K2_Y,"退出");
	GUI_PutString(COOR_K3_X,COOR_K3_Y,"查找");
	GUI_Rectangle (8,32,312,90,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_PutStringBack(20,24," 容抗测量 ");
	GUI_Rectangle (8,100,312,200,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_PutStringBack(20,92," 会话提示框 ");
	_UM_EarthSearchStatus();
}
/* 显示背景加显示数字 */
void _UM_EarthSearchShowValue (void)
{
	GUI_MenuDrawTitle (&value[6]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.CR_SYS > 999){
		SIGPRC_Struct.CR_SYS = 1000;
		GUI_PrintfBack_Rect (" >999千欧");
	}else if(SIGPRC_Struct.CR_SYS < 1){
		GUI_PrintfBack_Rect ("   <1千欧");
	}else{
		sprintf(printf_buf,"  %3d千欧",(int32_t)SIGPRC_Struct.CR_SYS);
		GUI_PrintfBack_Rect (printf_buf);
	}
	
	GUI_MenuDrawTitle (&value[7]);
	if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
		GUI_SetColor (RED, LIGHTGRAY);
	}else{
		GUI_SetColor (BLACK, LIGHTGRAY);
	}
	if(SIGPRC_Struct.RR_SYS > 999){
		SIGPRC_Struct.RR_SYS = 1000;
		GUI_PrintfBack_Rect (" >999千欧");
	}else if(SIGPRC_Struct.RR_SYS < 1){
		GUI_PrintfBack_Rect ("   <1千欧");
	}else{
		sprintf(printf_buf,"  %3d千欧",(int32_t)SIGPRC_Struct.RR_SYS);
		GUI_PrintfBack_Rect (printf_buf);
	}
	_UM_EarthSearchStatus();
}
void _UM_EarthSearchProcessInit (void)
{
	SIGPRC_Struct.status |= SIG_STAT_READY;
	SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
								SIG_STAT_MEASURING |
								SIG_STAT_EARTHLINE |
								SIG_STAT_CANCEL);
	SIGPRC_Struct.RR_SYS = 0;
	SIGPRC_Struct.CR_SYS = 0;
	_UM_EarthSearchShowBack();
}
static void _UM_EarthSearchProcessKey (uint8_t keymsg)
{
	switch(keymsg){
		case VK_BACK:
			if(SIGPRC_Struct.status & SIG_STAT_ERSCH){
				SIG_StopConverEarthSchCollect();
				SIGPRC_Struct.status |= SIG_STAT_CANCEL;
				SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
											SIG_STAT_MEASURING |
											SIG_STAT_EARTHLINE |
											SIG_STAT_READY);
				GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
				Protocol_SetSlaveAbort();
				_UM_EarthSearchShowBack ();
			}else{
				
				GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET, 0);
			}
			break;
		case VK_BACK| VK_LOOSE:
			if(SIGPRC_Struct.status & SIG_STAT_ERSCH){
				SIG_StopConverEarthSchCollect();
				SIGPRC_Struct.status |= SIG_STAT_CANCEL;
				SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
											SIG_STAT_MEASURING |
											SIG_STAT_EARTHLINE |
											SIG_STAT_READY);
				GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
				_UM_EarthSearchShowBack ();
			}
			break;
		case VK_ENTER:
			if(SIGPRC_Struct.status & SIG_STAT_ERSCH){
			}else{
				_UM_EarthSearchProcessInit();
				SIGPRC_Struct.status |= SIG_STAT_MEASURING;
				SIGPRC_Struct.status &= ~(SIG_STAT_MEASUREOK |
											SIG_STAT_CANCEL |
											SIG_STAT_EARTHLINE |
											SIG_STAT_READY);
				_UM_EarthSearchStatus ();
				SIG_StartConverEarthSchCollect ();
			}
			break;
	}
}
void _UM_EarthSearchProcessExit (void)
{
	SIGPRC_Struct.RR_SYS = 0;
	SIGPRC_Struct.CR_SYS = 0;
}
uint8_t _UM_EarthSearchProcessService (WIDGET_SERVICE_NO no, WIDGET *p)
{
	uint8_t keymsg,ret=FALSE;
	switch(no){
		case WIDGET_INIT_WIDGET:
			_UM_EarthSearchProcessInit ();
			break;
		case WIDGET_EXIT_WIDGET:
			_UM_EarthSearchProcessExit ();
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			GUI_WidgetProcessService (WIDGET_EXIT_WIDGET, 0);
			GUI_WidgetCloseFocus ();
			break;
		case WIDGET_OPEN_WIDGET:
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetDirectProcessService (WIDGET_UPDATA_FOCUSWIDGET,
											0);
			_UM_EarthSearchShowValue ();
			break;
		case WIDGET_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			if(keymsg == VK_NOKEY){
			}else{
				_UM_EarthSearchProcessKey (keymsg);
			}
			break;
		case WIDGET_UPDATA_DATA:
			_UM_EarthSearchShowValue ();
		default:
			break;
	}
	ret = TRUE;
	return ret;
}
void _UM_EorShowValue (void)
{
	GUI_SetAscType (FONT_ASC_16);
	GUI_SetHzkType (FONT_HZK_16);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_GEZ/1000.0);
	GUI_PutStringBack (8+16*7,menu4[1].title->coor.y,printf_buf);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_GEF/1000.0);
	GUI_PutStringBack (8+16*7,menu4[2].title->coor.y,printf_buf);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_RZ/1000.0);
	GUI_PutStringBack (8+16*7,menu4[3].title->coor.y,printf_buf);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_RF/1000.0);
	GUI_PutStringBack (8+16*7,menu4[4].title->coor.y,printf_buf);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_RR/1000.0);
	GUI_PutStringBack (8+16*7,menu4[5].title->coor.y,printf_buf);
	
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_CAP[0]/1000.0);
	GUI_PutStringBack (8+16*7,menu4[6].title->coor.y,printf_buf);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_CAP[1]/1000.0);
	GUI_PutStringBack (8+16*7,menu4[7].title->coor.y,printf_buf);
	sprintf(printf_buf,"%06.3f",
			SIGPRC_Struct.SIG_ERRCOE_CAP[2]/1000.0);
	GUI_PutStringBack (8+16*7,menu4[8].title->coor.y,printf_buf);/*
	if(wm[focus_widget_no]->focus_menu == &menu4[0]){
		GUI_PutStringBack (8,165,
				"输出三个端子短路");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[1]){
		GUI_PutStringBack (8,165,
				"负与地短路\n正与地接电池\n测得母线电压");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[2]){
		GUI_PutStringBack (8,165,
				"正与地短路\n负与地接电池\n测得母线电压");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[3]){
		GUI_PutStringBack (8,165,
				"正电阻的误差");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[4]){
		GUI_PutStringBack (8,165,
				"负电阻的误差");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[5]){
		GUI_PutStringBack (8,165,
				"阻抗修正系数");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[6]){
		GUI_PutStringBack (8,165,
				"用10uf小电容修正");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[7]){
		GUI_PutStringBack (8,165,
				"用200uf电容修正");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[8]){
		GUI_PutStringBack (8,165,
				"用1000uf电容修正");
	}else if(wm[focus_widget_no]->focus_menu == &menu4[9]){
		GUI_PutStringBack (8,165,
				"初始化修正系数");
	}*/
}
uint32_t coebit = 0;
void Menu_ServiceZeroKeymsgProcess (uint8_t keymsg)
{
}
void Menu_ServiceZeroProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->
								focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					SIG_ZeroDiff ();
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_CorrectPara (volatile int32_t *p,int16_t x, int16_t y)
{
	uint8_t pb=0;
	uint8_t keymsg;
	while(1){
		if(pb==0){
			sprintf(printf_buf,"%u",*p%10);
		}else if(pb==1){
			sprintf(printf_buf,"%u",*p/10%10);
		}else if(pb==2){
			sprintf(printf_buf,"%u",*p/100l%10);
		}else if(pb==3){
			sprintf(printf_buf,"%u",*p/1000l%10);
		}else if(pb==4){
			sprintf(printf_buf,"%u",*p/10000l%10);
		}
		GUI_SetAscType (FONT_ASC_16);
		GUI_SetHzkType (FONT_HZK_16);
		if(pb < 3){
			GUI_PutStringBack_Rect(x+(5-pb)*8,y,printf_buf);
		}else{
			GUI_PutStringBack_Rect(x+(4-pb)*8,y,printf_buf);
		}
		keymsg = GUI_Get_Keymsg ();
		if(keymsg == (VK_ENTER)){
			wm[focus_widget_no]->service(WIDGET_UPDATA_FOCUSWIDGET,0);
			pb ++;
			if(pb > 4){
				SIG_SaveSystemParam();
				break;
			}
		}else if(keymsg == (VK_BACK)){
			wm[focus_widget_no]->service(WIDGET_UPDATA_FOCUSWIDGET,0);
			if(pb == 0){
				SIG_SaveSystemParam();
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
void Menu_ServiceGEZProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_GEZ,
							8+16*7,16);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceGEFProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET, 0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_GEF,
							8+16*7,32);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceRZProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET, 0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_RZ,
							8+16*7,48);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceRFProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_RF,
							8+16*7,64);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceCPProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_CP,
							8+16*7,80);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceCRProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_CR,
							8+16*7,112);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceCAP1XProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_CAP[0],
							8+16*7,menu4[6].title->coor.y);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceCAP2XProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_CAP[1],
							8+16*7,menu4[7].title->coor.y);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceCAP3XProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_CAP[2],
							8+16*7,menu4[8].title->coor.y);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceRRProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectPara (&SIGPRC_Struct.SIG_ERRCOE_RR,
							8+16*7,menu4[5].title->coor.y);
					break;
			}
			break;
		default:
			break;
	}
}
void Menu_ServiceRESETProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->
									focus_menu);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					SIG_SystemParamInit ();
					_UM_EorShowValue ();
					break;
			}
			break;
		default:
			break;
	}
}
static void _UM_EarthEorProcessKey (uint8_t keymsg)
{
	switch(keymsg){
		case VK_DOWN:
			wm[focus_widget_no]->focus_menu = 
				wm[focus_widget_no]->focus_menu->next;
			GUI_MenuDrawShowAllFocus (wm[focus_widget_no]->focus_menu);
			break;
		case VK_BACK:
			GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,
									0);
			break;
		case VK_ENTER:
			break;
	}
}
uint8_t _UM_EarthEorProcessService (WIDGET_SERVICE_NO no,WIDGET *p)
{
	uint8_t keymsg,ret=FALSE;
	switch(no){
		case WIDGET_INIT_WIDGET:
			_UM_EorShowValue ();
			break;
		case WIDGET_EXIT_WIDGET:
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			GUI_WidgetProcessService (WIDGET_EXIT_WIDGET, 0);
			GUI_WidgetCloseFocus ();
			break;
		case WIDGET_OPEN_WIDGET:
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetDirectProcessService (WIDGET_UPDATA_FOCUSWIDGET,
											0);
			_UM_EorShowValue ();
			break;
		case WIDGET_PROCESS_KEYMSG:
			if(wm[focus_widget_no]->focus_menu->service == NULL){
				keymsg = GUI_Get_Keymsg ();
				if(keymsg == VK_NOKEY){
				}else{
					_UM_EarthEorProcessKey (keymsg);
				}
			}else{
				wm[focus_widget_no]->focus_menu->
						service (MENU_PROCESS_KEYMSG,0);
			}
			break;
		default:
			break;
	}
	ret = TRUE;
	return ret;
}
#include "../../devdriver/rtc/realtime.h"
#include "../../bsp/fwlib/adc.h"
void _UM_SystemBackground (void)
{
	uint32_t l;
	GUI_SetHzkType (FONT_HZK_24);
	GUI_SetAscType (FONT_ASC_24);
	GUI_SetColor(BLACK,DEFAULT_BACK);
	GUI_PutStringBack (18, title[19].coor.y, "日期:");
	GUI_PutStringBack (18, title[22].coor.y, "时间:");
	GUI_SetHzkType (FONT_HZK_16);
	GUI_SetAscType (FONT_ASC_16);
	sprintf(printf_buf,"%04d",RTC_Struct.tm_year);
	if(wm[focus_widget_no]->focus_menu == &menu5[0])
		GUI_SetColor(BLACK,ORINGE);
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[19].coor.x - 40, title[19].coor.y+4,
				printf_buf);
	sprintf(printf_buf,"  %02d",RTC_Struct.tm_mon);
	if(wm[focus_widget_no]->focus_menu == &menu5[1])
		GUI_SetColor(BLACK,ORINGE);
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[20].coor.x - 40, title[20].coor.y+4,
				printf_buf);
	sprintf(printf_buf,"  %02d",RTC_Struct.tm_mday);
	if(wm[focus_widget_no]->focus_menu == &menu5[2])
		GUI_SetColor(BLACK,ORINGE);		
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[21].coor.x - 40, title[21].coor.y+4,
				printf_buf);
	sprintf(printf_buf,"  %02d",RTC_Struct.tm_hour);
	if(wm[focus_widget_no]->focus_menu == &menu5[3])
		GUI_SetColor(BLACK,ORINGE);		
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[22].coor.x - 40, title[22].coor.y+4,
				printf_buf);
	sprintf(printf_buf,"  %02d",RTC_Struct.tm_min);
	if(wm[focus_widget_no]->focus_menu == &menu5[4])
		GUI_SetColor(BLACK,ORINGE);		
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[23].coor.x - 40, title[23].coor.y+4,
				printf_buf);
	sprintf(printf_buf,"  %02d",RTC_Struct.tm_sec);
	if(wm[focus_widget_no]->focus_menu == &menu5[5])
		GUI_SetColor(BLACK,ORINGE);
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[24].coor.x - 40, title[24].coor.y+4,
				printf_buf);

	l = UM_GET_SYSTEMPARA;
	l = l&PARA_LIGHT_MASK;
	if(l == 0X0){
		sprintf(printf_buf,"    最暗    ");
	}else if(l == 0X1){
		sprintf(printf_buf,"     暗     ");
	}else if(l == 0X2){
		sprintf(printf_buf,"     亮     ");
	}else{
		sprintf(printf_buf,"    最亮    ");
	}
	if(wm[focus_widget_no]->focus_menu == &menu5[6])
		GUI_SetColor(BLACK,ORINGE);		
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[25].coor.x + 5*24, title[25].coor.y+4,
				printf_buf);
	l = UM_GET_SYSTEMLIGHTTIME;
	if(l >= 3600){
		sprintf(printf_buf,"    常亮    ");
	}else if (l<60){
		sprintf(printf_buf,"    %02u秒    ",l);	
	}else{
		sprintf(printf_buf,"    %02u分    ",l/60);
	}
	if(wm[focus_widget_no]->focus_menu == &menu5[7])
		GUI_SetColor(BLACK,ORINGE);
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[26].coor.x + 5*24, title[26].coor.y+4,
				printf_buf);
	l = UM_GET_SYSTEMPARA;
	if(l&PARA_BEEP_MASK){
		sprintf(printf_buf,"    打开    ");
	}else{
		sprintf(printf_buf,"    关闭    ");
	}
	if(wm[focus_widget_no]->focus_menu == &menu5[8])
		GUI_SetColor(BLACK,ORINGE);
	else
		GUI_SetColor(BLACK,LIGHTGRAY);
	GUI_PutStringBack_Rect_24 (title[28].coor.x + 5*24, title[28].coor.y+4,
				printf_buf);

}
void _UM_SystemSetWidgetInit (void)
{
	uint16_t x,y;
	GUI_SetHzkType (FONT_HZK_16);
	GUI_SetAscType (FONT_ASC_16);
	for(y=224; y<240; y++){
		for(x=0; x<320; x++){
			API_GUI_Point (x, y, UNDER_COLU_COLOR);
		}
	}
	GUI_PutString(COOR_K2_X-32,COOR_K2_Y,"保存/退出");
	GUI_PutString(COOR_K3_X,COOR_K3_Y,"修改");
	GUI_PutString(COOR_K1_X,COOR_K1_Y,"选择");
	GUI_Rectangle (8,8,312,90,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_SetHzkType (FONT_HZK_16);
	GUI_SetAscType (FONT_ASC_16);
	GUI_PutStringBack(20,0," 时间设置 ");
	GUI_Rectangle (8,98,312,200,BLACK);
	GUI_SetColor (BLACK, DEFAULT_BACK);
	GUI_SetHzkType (FONT_HZK_16);
	GUI_SetAscType (FONT_ASC_16);
	GUI_PutStringBack(20,91," 屏幕设置 ");
	_UM_SystemBackground ();
}
void Menu_CorrectTimeParc (void)
{
	uint8_t pb=0;
	uint8_t keymsg;
	unsigned int timecounter;
	sprintf(printf_buf,"%04d-%02d-%02d %02d:%02d",RTC_Struct.tm_year,
			RTC_Struct.tm_mon,RTC_Struct.tm_mday,
			RTC_Struct.tm_hour,RTC_Struct.tm_min);
	GUI_SetAscType(FONT_ASC_24);
	GUI_SetHzkType(FONT_HZK_24);
	GUI_SetColor(GREEN, WHITE);
	GUI_PutString (10+24*4,24,printf_buf);
	while(1){
		GUI_SetColor(GREEN, WHITE);
		switch(pb){
			case 0:
				sprintf(printf_buf,"%02d",RTC_Struct.tm_min);
				GUI_PutStringBack (106+12*14,24,printf_buf);
				break;
			case 1:
				sprintf(printf_buf,"%02d",RTC_Struct.tm_hour);
				GUI_PutStringBack (106+12*11,24,printf_buf);
				break;
			case 2:
				sprintf(printf_buf,"%02d",RTC_Struct.tm_mday);
				GUI_PutStringBack (106+12*8,24,printf_buf);
				break;
			case 3:
				sprintf(printf_buf,"%02d",RTC_Struct.tm_mon);
				GUI_PutStringBack (106+12*5,24,printf_buf);
				break;
			case 4:
				sprintf(printf_buf,"%04d",RTC_Struct.tm_year);
				GUI_PutStringBack (106+12*0,24,printf_buf);
				break;
		}
		keymsg = GUI_Get_Keymsg ();
		if(keymsg == (VK_ENTER)){
			wm[focus_widget_no]->service(WIDGET_UPDATA_FOCUSWIDGET,0);
			pb ++;
			if(pb >= 5){
				RTC_CorrectDay (&RTC_Struct);
				timecounter = RTC_mktimev (&RTC_Struct);
				RTC_to_tm (timecounter, &RTC_Struct);
				RTC_SetCounter (timecounter);	
				break;
			}
		}else if(keymsg == (VK_BACK)){
			wm[focus_widget_no]->service(WIDGET_UPDATA_FOCUSWIDGET,0);
			if(pb == 0){
				break;
			}
			pb --;
		}else if(keymsg == (VK_DOWN)){
			switch(pb){
				case 0:
					if(RTC_Struct.tm_min >0){
						RTC_Struct.tm_min--;
					}else{
						RTC_Struct.tm_min = 59;
					}
					break;
				case 1:
					if(RTC_Struct.tm_hour >0){
						RTC_Struct.tm_hour--;
					}else{
						RTC_Struct.tm_hour = 23;
					}
					break;
				case 2:
					if(RTC_Struct.tm_mday >0){
						RTC_Struct.tm_mday--;
					}else{
						RTC_Struct.tm_mday = 31;
					}
					break;
				case 3:
					if(RTC_Struct.tm_mon >0){
						RTC_Struct.tm_mon--;
					}else{
						RTC_Struct.tm_mon = 12;
					}
					break;
				case 4:
					if(RTC_Struct.tm_year >2000){
						RTC_Struct.tm_year--;
					}else{
						RTC_Struct.tm_year = 2099;
					}
					break;
			}
		}
	}
}
void Menu_ServiceYearProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t timecounter;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();					
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					RTC_Struct.tm_year++;
					if(RTC_Struct.tm_year>=2100){
						RTC_Struct.tm_year = 1970;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);					
					_UM_SystemBackground ();
					break;
				case VK_ENTER| VK_CONT:
					RTC_Struct.tm_year += 10;
					if(RTC_Struct.tm_year>=2100){
						RTC_Struct.tm_year = 1970;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);					
					_UM_SystemBackground ();
					break;					
			}
			break;
	}	
}
void Menu_ServiceMonProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t timecounter;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					RTC_Struct.tm_mon++;
					if(RTC_Struct.tm_mon >= 13){
						RTC_Struct.tm_mon = 1;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);					
					_UM_SystemBackground ();
					break;
				case VK_ENTER| VK_CONT:
					RTC_Struct.tm_mon += 10;
					if(RTC_Struct.tm_mon >= 13){
						RTC_Struct.tm_mon = 1;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);					
					_UM_SystemBackground ();
					break;
			}
			break;
	}
}
void Menu_ServiceDayProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t timecounter;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					RTC_Struct.tm_mday++;
					if(RTC_Struct.tm_mday >= 32){
						RTC_Struct.tm_mday = 1;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);					
					_UM_SystemBackground ();
					break;
				case VK_ENTER| VK_CONT:
					RTC_Struct.tm_mday += 10;
					if(RTC_Struct.tm_mday >= 32){
						RTC_Struct.tm_mday = 1;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);					
					_UM_SystemBackground ();
					break;					
			}
			break;
	}
}
void Menu_ServiceHourProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t timecounter;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					RTC_Struct.tm_hour ++;
					if(RTC_Struct.tm_hour >= 24){
						RTC_Struct.tm_hour = 0;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);
					_UM_SystemBackground ();
					break;
				case VK_ENTER| VK_CONT:
					RTC_Struct.tm_hour+=10;
					if(RTC_Struct.tm_hour >= 24){
						RTC_Struct.tm_hour = 0;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);
					_UM_SystemBackground ();
					break;
			}
			break;
	}
}
void Menu_ServiceMinProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t timecounter;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					RTC_Struct.tm_min++;
					if(RTC_Struct.tm_min >= 60){
						RTC_Struct.tm_min = 0;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);
					_UM_SystemBackground ();
					break;
				case VK_ENTER| VK_CONT:
					RTC_Struct.tm_min += 10;
					if(RTC_Struct.tm_min >= 60){
						RTC_Struct.tm_min = 0;
					}
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);
					_UM_SystemBackground ();
					break;					
			}
			break;
	}
}
void Menu_ServiceSecProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t timecounter;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					RTC_Struct.tm_sec++;
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);
					_UM_SystemBackground ();
					break;
				case VK_ENTER| VK_CONT:
					RTC_Struct.tm_sec += 10;
					RTC_CorrectDay (&RTC_Struct);
					timecounter = RTC_mktimev (&RTC_Struct);
					RTC_to_tm (timecounter, &RTC_Struct);
					RTC_SetCounter (timecounter);
					_UM_SystemBackground ();
					break;					
			}
			break;
	}
}
void Menu_ServiceBeepProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
						UM_SET_SYSTEMPARA (UM_GET_SYSTEMPARA&
												(~PARA_BEEP_MASK));
					}else{
						UM_SET_SYSTEMPARA (UM_GET_SYSTEMPARA|
												(PARA_BEEP_MASK));
					}
					_UM_SystemBackground ();
					break;
			}
			break;
	}
}
void Menu_ServiceTimeProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					wm[focus_widget_no]->focus_menu = 
						wm[focus_widget_no]->focus_menu->next;
					GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					Menu_CorrectTimeParc();
					break;
			}
			break;
	}
}
void Menu_ServiceLightLevelProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t l;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					l = UM_GET_SYSTEMPARA;
					l &= PARA_LIGHT_MASK;
					if(l>=3){
						l = 0;
					}else{
						l++;
					}
					UM_SET_SYSTEMPARA ((UM_GET_SYSTEMPARA&
											(~PARA_LIGHT_MASK))|l);
					API_GUI_SetBackLightLevel(l);
					_UM_SystemBackground ();
					break;
			}
			break;			
	}
}
void Menu_ServiceLightTimeProcess (MENU_SERVICE_NO service_no,
				const struct MENU_STRUCT *p)
{
	uint8_t keymsg;
	uint32_t l;
	switch(service_no){
		case MENU_PROCESS_KEYMSG:
			keymsg = GUI_Get_Keymsg ();
			switch(keymsg){
				case VK_DOWN:
					GUI_WidgetJumpNextMenu ();
					GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
					_UM_SystemBackground ();
					break;
				case VK_BACK:
					GUI_WidgetProcessService (WIDGET_CLOSE_FOCUSWIDGET,0);
					break;
				case VK_ENTER:
					l = UM_GET_SYSTEMLIGHTTIME;
					if(l == 15){
						l = 30;
					}else if(l==30){
						l = 60;
					}else if(l==60){
						l = 300;
					}else if(l==300){
						l = 900;
					}else if(l==900){
						l = 1800;
					}else if(l==1800){
						l = 3600;
					}else{
						l = 15;
					}
					UM_SET_SYSTEMLIGHTTIME (l);
					_UM_SystemBackground ();
					//GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
					break;
			}
			break;			
	}
}
extern volatile long jiffies;
long updata_initjif = 0;
uint8_t _UM_SystemSetProcessService (WIDGET_SERVICE_NO no,WIDGET *p)
{
	uint8_t keymsg,ret=FALSE;
	switch(no){
		case WIDGET_INIT_WIDGET:
			_UM_SystemSetWidgetInit();
			GUI_MenuCancelFocus (wm[focus_widget_no]->focus_menu);
			break;
		case WIDGET_EXIT_WIDGET:
			break;
		case WIDGET_UPDATA_ALLWIDGET:
			GUI_WidgetUpdataAll ();
			break;
		case WIDGET_CLOSE_FOCUSWIDGET:
			GUI_WidgetProcessService (WIDGET_EXIT_WIDGET, 0);
			GUI_WidgetCloseFocus ();
			break;
		case WIDGET_OPEN_WIDGET:
			break;
		case WIDGET_UPDATA_FOCUSWIDGET:
			GUI_WidgetDirectProcessService (WIDGET_UPDATA_FOCUSWIDGET,
											0);
			GUI_WidgetProcessService (WIDGET_INIT_WIDGET,0);
			break;
		case WIDGET_PROCESS_KEYMSG:
			if(updata_initjif < jiffies){
				_UM_SystemBackground ();
				updata_initjif += 80;
			}
			if(wm[focus_widget_no]->focus_menu->service == NULL){
				keymsg = GUI_Get_Keymsg ();
				if(keymsg == VK_NOKEY){
				}else{
				}
			}else{
				wm[focus_widget_no]->focus_menu->
						service (MENU_PROCESS_KEYMSG,0);
			}
			break;
		default:
			break;
	}
	ret = TRUE;
	return ret;
}
const uint8_t bat0[2+16*12];
const uint8_t bat1[2+16*12];
const uint8_t bat2[2+16*12];
const uint8_t bat3[2+16*12];
const uint8_t bat4[2+16*12];
const uint8_t bat5[2+16*12];
const uint8_t bat6[2+16*12];
#define BAT1 32000
#define BAT2 33000
#define BAT3 34000
#define BAT4 35000
#define BAT5 36000
//#define BAT6 37000
void _UM_ShowDefault_Background(void)
{
	uint16_t value;
	GUI_WindowDraw (&win[1]);
	GUI_SetAscType(FONT_ASC_16);
	GUI_SetHzkType(FONT_HZK_16);
	GUI_SetColor(BLUE, LGRAYBLUE);
	if(wm[focus_widget_no]==&widget[0]){
		GUI_PutString (0,0,"※欢迎使用");
	}else if(wm[focus_widget_no]==&widget[1]){
		GUI_PutString (0,0,"※电阻测量");
	}else if(wm[focus_widget_no]==&widget[2]){
		GUI_PutString (0,0,"※阻抗测量");
	}else if(wm[focus_widget_no]==&widget[3]){
		GUI_PutString (0,0,"※环网查找");
	}else if(wm[focus_widget_no]==&widget[5]){
		GUI_PutString (0,0,"※系统设置");
	}
	sprintf(printf_buf,"%04d/%02d/%02d %02d:%02d",
				RTC_Struct.tm_year,RTC_Struct.tm_mon,
				RTC_Struct.tm_mday,RTC_Struct.tm_hour,
				RTC_Struct.tm_min);	
	GUI_PutString (184,0,printf_buf);
	value = ADC_GetConversionValue (ADC0_SE12_PB2);
	//printf("\r\nvalue:%u",value);
	//sprintf(printf_buf,"BAT:100 %u",value);
	GUI_PutString (108,0,"BAT:");
	if(value < BAT1){
		GUI_LoadCharStr(140,2,bat1);
	}else if(value < BAT2){
		GUI_LoadCharStr(140,2,bat2);
	}else if(value < BAT3){
		GUI_LoadCharStr(140,2,bat3);
	}else if(value < BAT4){
		GUI_LoadCharStr(140,2,bat4);
	}else{
		GUI_LoadCharStr(140,2,bat5);
	}
	
	GUI_Set_X0_Y0_Base (&wm[focus_widget_no]->win->point);
}
void _UM_ShowMainWidget_Background (void)
{
	uint16_t x;
	uint16_t y;
	_UM_ShowBackGround ();	
	for(y=0; y<224; y++){
		for(x=0; x<320; x++){
			API_GUI_Point (x, y, DEFAULT_BACK);
		}
	}
	for(; y<240; y++){
		for(x=0; x<320; x++){
			API_GUI_Point (x, y, UNDER_COLU_COLOR);
		}
	}
	GUI_SetAscType(FONT_ASC_24);
	GUI_SetHzkType(FONT_HZK_24);
	GUI_SetColor(BLACK, BLUE);
	GUI_PutString(16,24,"直\n流\n电\n源\n带\n电");
	GUI_PutString(40,54,"解\n环\n仪");
	GUI_SetAscType(FONT_ASC_24);
	GUI_SetHzkType(FONT_HZK_24);
	GUI_SetColor(BLACK, BLUE);
	GUI_PutString(64,32,"︵\n信\n号\n源\n︶");
	GUI_SetAscType(FONT_ASC_16);
	GUI_SetHzkType(FONT_HZK_16);
	GUI_SetColor(BLACK, BLUE);
	GUI_PutString(COOR_K1_X,COOR_K1_Y,"选择");
	GUI_PutString(COOR_K3_X,COOR_K3_Y,"进入");
	_UM_ShowDefault_Background();
}
void _UM_ShowBackGround (void)
{
//API_GUI_Point (int16_t x, int16_t y, TCOLOR color)
//uint16_t *p;
//	register uint32_t i;
//	p = (uint16_t *)ImagBackGround;	
//	i = 76800;
//	while (--i){
//		pSSD1906DAT[i] = *p;
//		p++;
//	}
}
const uint8_t bat0[2+16*12]=
{
16,12,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
const uint8_t bat1[2+16*12]=
{
16,12,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
const uint8_t bat2[2+16*12]=
{
16,12,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
const uint8_t bat3[2+16*12]=
{
16,12,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
const uint8_t bat4[2+16*12]=
{
16,12,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
const uint8_t bat5[2+16*12]=
{
16,12,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
};
/*********************************END OF FILE****************************/

