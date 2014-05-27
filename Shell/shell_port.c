/*************************************************************************
#    FileName: chshell_port.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-02-05 23:05:34
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include <stdio.h>
//这里包含你的串口驱动头文件
#include "../bsp/fwlib/uart.h"
#include "shell.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/

struct UARTSHELL_DEV SHL_Dev;
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 SHL_TestFun(u8 *str1, u8 num1, u8 *str2, u8 num2, u8 *str3)
{
	printf("进入TestFun函数\r\n");
	printf("参数1: %s\r\n",str1);
	printf("参数2: %d\r\n",num1);
	printf("参数3: %s\r\n",str2);
	printf("参数4: %d\r\n",num2);
	printf("参数5: %s\r\n",str3);
	return 100;
}
/**
  * @brief  函数管理窗口.
  * @param  None.
  * @retval None.
  * @verify .
  */
extern void WV_SetFrequency (u32 hz);
extern void WV_SetAmplitude (u32 mV);
extern void RL_ZRelayDisconnect (void);
extern void RL_ZRelayConnect (void);
extern void RL_FRelayDisconnect (void);
extern void RL_FRelayConnect (void);
extern void API_GUI_FillSCR (uint16_t dat);
extern void NVIC_SystemReset(void);
extern void ADG_SetChannel_DC (void);
extern void ADG_SetChannel_AC (void);
extern void SIG_StartConverCollect (void);
extern void SIG_PrintfAD (void);
extern void BSP_PowerDown (void);
extern void SIG_PrintfConverNum(uint8_t n);
extern void SIG_PrintfADvalue(void);
extern void ADG_SetChannel (uint8_t chn);
extern void SIG_PrintfSinCos(void);
void SHL_startac(void)
{
	WV_SetFrequency (5);
	WV_SetAmplitude (500);
	RL_ZRelayDisconnect();
	RL_FRelayDisconnect();
	ADG_SetChannel_AC ();
}
const struct UARTSHELL_FunTab SHL_FunTab[]=
{
	(u32*) SHL_TestFun,"u8 SHL_TestFun(u8* str1,u8 num1,u8* str2,u8 num2,u8* str3)",
	(void*) WV_SetFrequency,"setf(u32 hz)",
	(void*) WV_SetAmplitude,"seta(u32 mV)",
	(void*) RL_ZRelayDisconnect,"zrdis(void)",
	(void*) RL_FRelayDisconnect,"frdis(void)",
	(void*) RL_ZRelayConnect,"zrcne(void)",
	(void*) RL_FRelayConnect,"frcne(void)",
	(void*) API_GUI_FillSCR,"setcolor(u16 col)",
	(void*) NVIC_SystemReset,"sysreset(void)",
	(void*) ADG_SetChannel_DC,"setdc(void)",
	(void*) ADG_SetChannel_AC,"setac(void)",
	(void*) SIG_StartConverCollect,"startconver(void)",
	(void*) SIG_PrintfAD,"printfad(void)",
	(void*) BSP_PowerDown,"powerdown(void)",
	(void*) SHL_startac,"startac(void)",
	(void*) SIG_PrintfConverNum,"pad(u8 n)",
	(void*) SIG_PrintfADvalue,"p(void)",
	(void*) ADG_SetChannel,"chn(u8 n)",
	(void*) SIG_PrintfSinCos,"pcs(void)",
};	
/**
  * @brief  SHELL组件初始化.
  * @param  None.
  * @retval None.
  * @verify .
  */
void SHL_Init(void)
{
	//UART_Init(UART3,115200l); //初始化调试串口 //默认 UART3 115200 在UART.H中定义
	SHL_Dev.cnum = 0;
	SHL_Dev.funs = SHL_FunTab;
	SHL_Dev.USART_STAT = 0;
	SHL_Dev.fnum = sizeof(SHL_FunTab)/sizeof(struct UARTSHELL_FunTab);
	
}
/*********************************END OF FILE****************************/


