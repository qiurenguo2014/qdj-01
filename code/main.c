/*========================================================================
#   FileName: main.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-16 10:05:50
========================================================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "devdriver/systick/systick.h"
#include "devdriver/relay/relay.h"
#include "devdriver/ssd1906/ssd1906_hw_fb16b.h"
#include "devdriver/rtc/realtime.h"
#include "bsp/bsp.h"
#include "gui/gui_keyboard.h"
#include "gui/lcddrive.h"
#include "gui/windows.h"
#include "gui/widget.h"
#include "app/menu/usermenu.h"
#include "app/signal/sig_process.h"
#include "app/menu/usermenu.h"
#include "app/wifi/protocol.h"
/* Typedef -------------------------------------------------------------------*/
/* Define --------------------------------------------------------------------*/
/* Macro ---------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/
void Thread_KeyProcess (void);
void Thread_RelayProcess (void);
void Thread_SignalProcess (void);
void Thread_GuiProcess (void);
void Thread_ShowTime (void);
/* Functions -----------------------------------------------------------------*/
int main(int argc, char **argv)
{
	static jif_dog = 0;
	BSP_Configuration ();
	UM_Configuration ();
	SIG_Init ();

	while(1){
		/* 按键处理 */
		Thread_KeyProcess ();
		/* 继电器处理 */
		Thread_RelayProcess ();
		/* 信号处理 */
		Thread_SignalProcess ();
		/* 显示时间 */
		Thread_ShowTime ();
		if(jif_dog < jiffies){
			WDOG_Feed();
			jif_dog = jiffies + 1000;
		}		
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
extern void _UM_ShowDefault_Background(void);
extern volatile uint8_t vk_msg;
void Thread_ShowTime (void)
{
	static long jif=0;
	static long jif_bl=0;
	if(jif_bl == 0){
		jif_bl = jiffies + UM_GET_SYSTEMLIGHTTIME*100;
		API_GUI_SetBackLightLevel(UM_GET_SYSTEMPARA&PARA_LIGHT_MASK);
	}
	if(jif<jiffies){
		RTC_GetTime ();
		_UM_ShowDefault_Background ();
		jif = jiffies + 100;
	}
	if(UM_GET_SYSTEMLIGHTTIME >= 3600){
	}else{
		if(jif_bl < jiffies){
			if(SIGPRC_Struct.status & SIG_STAT_ERCLG||
					SIGPRC_Struct.status & SIG_STAT_ECCLG||
					SIGPRC_Struct.status & SIG_STAT_ERSCH){
				jif_bl = jiffies + UM_GET_SYSTEMLIGHTTIME*100;
			}else{
				API_GUI_SetBackLightLevel(0);
			}
		}else{
		}
		if(vk_msg != VK_NOKEY){
			jif_bl = jiffies + UM_GET_SYSTEMLIGHTTIME*100;
			API_GUI_SetBackLightLevel(UM_GET_SYSTEMPARA&PARA_LIGHT_MASK);
		}
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void Thread_SignalProcess (void)
{
	SIG_Thread_Sample ();
}
void Thread_RelayProcess (void)
{
	if (relay.status & RELAY_STTS_UPDATA){
		if (relay.jif_value < jiffies){
			RL_FRelayHiZ ();
			RL_ZRelayHiZ ();
		}
	}
}

void Thread_KeyProcess (void)
{
	GUI_WidgetProcessService (WIDGET_PROCESS_KEYMSG,0);
}
void assert_failed(uint8_t* file, uint32_t line)
{
//断言失败检测
// 	UART_printf("assert_failed:line:%d %s\r\n",line,file);
// 	while(1);
}

