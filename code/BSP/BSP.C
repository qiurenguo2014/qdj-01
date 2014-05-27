/*========================================================================
#   FileName: BSP.C
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-18 15:50:31
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "../devdriver/systick/systick.h"
#include "../devdriver/ssd1906/ssd1906_hw_fb16b.h"
#include "../devdriver/ssd1906/ligsen.h"
#include "../devdriver/ili9340/ili9340.h"
#include "../devdriver/wfg/wave.h"
#include "../devdriver/ad7687/ad7687.h"
#include "../devdriver/relay/relay.h"
#include "../devdriver/adg/adg.h"
#include "../devdriver/rtc/realtime.h"

#include "../gui/lcddrive.h"
#include "../app/menu/usermenu.h"
#include "../app/wifi/rf.h"
#include "fwlib/gpio.h"
#include "bsp.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
static void _BSP_NvicInit (void);
static void DisplayCPUInfo(void);
void BSP_BatterCheckInit (void);
/* Functions --------------------------------------------------------*/
extern long speak_jif;
void BSP_Configuration (void)
{
	GetCPUInfo ();	
	BSP_PowerInit ();
	BSP_PowerDown ();
	BSP_PowerUp ();	
	_BSP_NvicInit ();	
	UART_Configuration ();
	DisplayCPUInfo ();	
	/* SSD1906驱动初始化 使其初始化为RGB模式 */
	ILI_Configuration ();
	LIG_Configuration ();
	SSD1906_Configurationg ();
	STK_Configuration ();
	/* 继电器配置成断开连接 */
	RL_Configuration ();
	if(UM_GET_SYSTEMPARA&PARA_BEEP_MASK){
		API_GUI_OpenSpeak ();
		speak_jif = jiffies+30;
	}
	/* AD7687初始化配置 SPI模式，SCK与WV_Configuration共用,FTM初始化,采用中断方式采集 */
	AD7687_Configuration ();
	/* 波形发生器初始化配置，配置ML2035和AD5453，采用模拟SPI，在使用时，将SCK初始化为IO模式，在恢复SPI */
	WV_Configuration ();
	/* ADG409模拟开关芯片配置，通道一 */
	ADG_Configuration ();
	/* RTC初始化 */
	RTC_Init ();
	/* BAT电量初始化 */
	BSP_BatterCheckInit ();
	RF_Init ();
	//SN74121_Init ();
	//STK_Configuration ();
	STK_delay10ms(50);
	GUI_Get_Keymsg ();
	WDOG_Init (60000);
}
#define POWER PTC
#define POWER_Pin GPIO_Pin_12
void BSP_PowerInit (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIOx = POWER;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = POWER_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_Init (&GPIO_InitStruct);
}
void BSP_PowerUp (void)
{
	POWER->PSOR |= (1<<GPIO_Pin_12);
}
void BSP_PowerDown (void)
{
	POWER->PCOR |= (1<<GPIO_Pin_12);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
#include "fwlib/adc.h"
#define BAT_ADC ADC0
#define BAT_PORT PORTB
#define BAT_PORT_Pin GPIO_Pin_2
void BSP_BatterCheckInit (void)
{
	unsigned short cal_var;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	BAT_PORT->PCR[BAT_PORT_Pin] &= ~PORT_PCR_MUX_MASK;
	BAT_PORT->PCR[BAT_PORT_Pin] |= PORT_PCR_MUX(0);
	BAT_ADC->CFG1 &= ~(ADC_CFG1_MODE_MASK);
	BAT_ADC->CFG1 |= ADC_CFG1_MODE (3);					//
	BAT_ADC->CFG1 &= ~(ADC_CFG1_ADICLK_MASK);
	BAT_ADC->CFG1 |= (ADC_CFG1_ADICLK (0));				//
	BAT_ADC->CFG1 &= ~(ADC_CFG1_ADLSMP_MASK);			//
	BAT_ADC->CFG1 &= ~(ADC_CFG1_ADIV_MASK);
	BAT_ADC->CFG1 |= ADC_CFG1_ADIV (3);					//
	BAT_ADC->CFG2 = 0;									//
	BAT_ADC->CFG2 |= (ADACKEN_DISABLED|ADHSC_HISPEED
			|ADC_CFG2_ADLSTS(ADLSTS_20));
	BAT_ADC->CV1 = 0x1234u; 
	BAT_ADC->CV2 = 0x5678u;
	BAT_ADC->SC2 = (ACFE_DISABLED|ACFGT_GREATER
			|ACREN_ENABLED|DMAEN_DISABLED
			|ADC_SC2_REFSEL(REFSEL_EXT));
	BAT_ADC->SC3 = (CAL_OFF|ADCO_SINGLE|AVGE_ENABLED
			|ADC_SC3_AVGS(AVGS_32));
	BAT_ADC->PGA = (PGAEN_DISABLED|PGACHP_NOCHOP|PGALP_NORMAL
			|ADC_PGA_PGAG(PGAG_64));
	BAT_ADC->SC1[0] &= ~ADC_SC1_DIFF_MASK;//
	
	BAT_ADC->SC2 &= ~ADC_SC2_ADTRG_MASK;
	BAT_ADC->SC3 &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); 
	BAT_ADC->SC3 |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(AVGS_32) );
	BAT_ADC->SC3 |= ADC_SC3_CAL_MASK ;
	while((BAT_ADC->SC1[0] & ADC_SC1_COCO_MASK) == 0);
	if((BAT_ADC->SC3 & ADC_SC3_CALF_MASK) == CALF_FAIL){		
	}else{
		cal_var = 0x00;
		cal_var =  BAT_ADC->CLP0; 
		cal_var += BAT_ADC->CLP1; 
		cal_var += BAT_ADC->CLP2; 
		cal_var += BAT_ADC->CLP3; 
		cal_var += BAT_ADC->CLP4; 
		cal_var += BAT_ADC->CLPS; 
		cal_var = cal_var/2;
		cal_var |= 0x8000; // Set MSB
		BAT_ADC->PG =  ADC_PG_PG(cal_var);
		// Calculate minus-side calibration
		cal_var = 0x00;
		cal_var =  BAT_ADC->CLM0; 
		cal_var += BAT_ADC->CLM1; 
		cal_var += BAT_ADC->CLM2; 
		cal_var += BAT_ADC->CLM3; 
		cal_var += BAT_ADC->CLM4; 
		cal_var += BAT_ADC->CLMS; 
		cal_var = cal_var/2;
		cal_var |= 0x8000; // Set MSB
		BAT_ADC->MG = ADC_MG_MG(cal_var); 
		BAT_ADC->SC3 &= ~ADC_SC3_CAL_MASK;
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _BSP_NvicInit (void)
{	
#ifdef VECT_TAB_RAM 
	SCB->VTOR = 0X1FFF0000;
#else /* VECT_TAB_FLASH */
	SCB->VTOR = 0x4000;
#endif
	NVIC_SetPriorityGrouping (5);//抢占式优先级(0~3),亚优先级(0~3)
	//NVIC_SetPriority(PORTA_IRQn,NVIC_EncodePriority(5,1,0));
	NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(5,1,1));
	NVIC_SetPriority(UART3_RX_TX_IRQn,NVIC_EncodePriority(5,1,2));
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(5,1,3));
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void DisplayCPUInfo(void)
{
	printf("-----------------------------------\r\n");
	//打印封装信息
	printf("PinCnt:			%d\r\n",CPUInfo.PinCnt);
	//打印SiliconRevID
	//printf("SiliconRevID:		%d.%d\r\n",CPUInfo.SiliconRev/10,CPUInfo.SiliconRev%10);
	//打印PFlash大小
	printf("PFlash Size: 		%dKB\r\n",CPUInfo.PFlashSize/1024);
	//打印FlexNVM大小
	printf("FlexNVM Size: 		%dKB\r\n",CPUInfo.FlexNVMSize/1024);
	//打印RAM 大小
	printf("RAM Size :		%dKB\r\n",CPUInfo.RAMSize/1024);
	//打印CoreClock
	printf("CoreClock: 		%dHz\r\n",CPUInfo.CoreClock);
	//打印BusClock
	printf("BusClock: 		%dHz\r\n",CPUInfo.BusClock);
	//打印FlexBusClock
	printf("FlexBusClock: 		%dHz\r\n",CPUInfo.FlexBusClock);
	//打印FlashClock
	printf("FlashClock: 		%dHz\r\n",CPUInfo.FlashClock);
	printf("Program date:		%s\r\n",__DATE__);
	printf("Program time:		%s\r\n",__TIME__);
	printf("---------------------------by Allen\r\n");
}
/**
  * @brief  power manager.
  * @param  None
  * @retval None
  */
int fgetc(FILE *f)
{
	while((UART3->S1 & UART_S1_RDRF_MASK) == 0);//判断接收缓冲区是否满
	return (int)UART3->D;
}
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	while(!(UART3->S1 & UART_S1_TDRE_MASK));
	UART3->D = (uint8_t)ch;
	return ch;
}

/*********************************END OF FILE**********************************/


/*	//接通NVIC上对应串口中断线
	NVIC_EnableIRQ(UART3_RX_TX_IRQn);
	NVIC_SetPriority(UART3_RX_TX_IRQn,NVIC_EncodePriority(5,1,2));
	UART_ITConfig(UART3,UART_IT_RDRF,ENABLE);*/
