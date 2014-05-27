/*========================================================================
#   FileName: wave.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-21 17:27:04
========================================================================*/
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "../../bsp/fwlib/gpio.h"
#include "../../bsp/fwlib/spi.h"
#include "../../bsp/fwlib/ftm.h"
#include "wave.h"
/* Typedef -------------------------------------------------------------------*/
/* Define --------------------------------------------------------------------*/
/* Macro ---------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/
static void GPIO_Configuration (void);
static void _WV_WriteMLReg (uint16_t dat);
static void _WV_WriteAD5453Reg (uint16_t dat);
static void FTM_Configuration (void);
/* Functions -----------------------------------------------------------------*/
/*ML2035 FREQ = (Fclkin * D15:D0)/2^23*/
void WV_Configuration (void)
{
	GPIO_Configuration ();
	FTM_Configuration ();
 	//_WV_WriteMLReg (0xf);
 	//_WV_WriteAD5453Reg (0x2Fff);
	WV_closeMl2035 ();
//  	while (1){
// 		b += 1000;
// 		_WV_WriteAD5453Reg (b);
// 		printf ("d:%u\r\n",b&0x3fff);
//  		a = 2500000;
//  		while (a--);
// 		a = 2500000;
//  		while (a--);
// 		a = 2500000;
//  		while (a--);
//  	}
}
/**
  * @brief  设置ML2035输出频率
  * 		f = (FCLKIN * D15:D0)/(2^23).
  * 		D15:D0 = (f*2^23)/FCLKIN
  * 		2^23 = 8388608
  * @param  None.
  * @retval None.
  * @verify .
  */
void WV_SetFrequency (uint32_t hz)
{
	uint16_t data;
	double tmp;
	tmp = 8388608l/FCLKIN;
	tmp *= hz;
	data = (uint16_t)tmp;
	_WV_WriteMLReg (data);
}
/**
  * @brief  设置输出幅值
  * 		14Bit DA输出 2^14 = 16384
  * 		2.5.
  * @param  None.
  * @retval None.
  * @verify .
  */
void WV_SetAmplitude (uint32_t mV)
{
	uint16_t data;
	double tmp;
	if (mV > 2500){		
	}else{
		tmp = mV/2500.0;
		tmp *= 16383;
		data = (uint16_t)tmp;
		_WV_WriteAD5453Reg (data);
	}	
}
static void _WV_delay100ns (void)
{
	uint32_t a = 20;
	while (a--);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _WV_WriteAD5453Reg (uint16_t dat)
{
	uint32_t i;
	uint16_t shiftbit = 0x8000;

	WV_RESET_AD5453_SYNC;
	_WV_delay100ns ();
	WV_SET_AD5453_SYNC;
	_WV_delay100ns ();
	WV_RESET_AD5453_SYNC;
	_WV_delay100ns ();
	
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(1);
 	WV_SCK->PDDR |= (1<<WV_SCK_Pin);
// 	WV_SCK_PORT->PCR[WV_SCK_Pin]&= ~PORT_PCR_ODE_MASK;

	for (i=0; i<16; i++){
		if (shiftbit & dat){
			WV_SET_AD5453_SDIN;
		}else{
			WV_RESET_AD5453_SDIN;
		}
		WV_SET_AD5453_SCLK;
		_WV_delay100ns ();
		WV_RESET_AD5453_SCLK;
		_WV_delay100ns ();
		shiftbit >>= 1;
	}
	WV_SET_AD5453_SYNC;
	
	GPIO_SetBits (WV_SCK, WV_SCK_Pin);
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(2);
}
void WV_closeMl2035 (void)
{
	uint32_t i;
	WV_SET_ML2035_LATI;
	_WV_delay100ns ();
	
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(1);
 	WV_SCK->PDDR |= (1<<WV_SCK_Pin);
// 	WV_SCK_PORT->PCR[WV_SCK_Pin]&= ~PORT_PCR_ODE_MASK;
	for (i=0; i<16; i++){
		WV_RESET_ML2035_SCK;
		WV_RESET_ML2035_SID;
		_WV_delay100ns ();
		WV_SET_ML2035_SCK;
		_WV_delay100ns ();
	}
	WV_RESET_ML2035_LATI;
	GPIO_SetBits (WV_SCK, WV_SCK_Pin);
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(2);
}
/**
  * @brief  IO鍙ｆ搷浣滈渶寤舵椂>100ns
  * 		SCK		  Digital input which clock in serial data on its rising edges.
  * 		SDI		  Serial data input
  * 		LATI(CS)  Digital input which latches serial data into the internal 
  * 				  data latch on falling edges.
  * @param  None
  * @retval None
  */
void _WV_WriteMLReg (uint16_t dat)
{
	uint16_t shiftbit = 0x1,i;
	WV_RESET_ML2035_LATI;
	_WV_delay100ns ();
	/*  */
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(1);
 	WV_SCK->PDDR |= (1<<WV_SCK_Pin);
// 	WV_SCK_PORT->PCR[WV_SCK_Pin]&= ~PORT_PCR_ODE_MASK;
	for (i=0; i<16; i++){
		WV_RESET_ML2035_SCK;
		if (shiftbit & dat){
			WV_SET_ML2035_SID;
		}else{
			WV_RESET_ML2035_SID;
		}
		_WV_delay100ns ();
		WV_SET_ML2035_SCK;
		_WV_delay100ns ();
		shiftbit <<= 1;
	}	
	WV_SET_ML2035_LATI;
	_WV_delay100ns ();
	WV_RESET_ML2035_LATI;
	/*  */
	GPIO_SetBits (WV_SCK, WV_SCK_Pin);
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(2);
}
#define FTM_PORT PORTA
#define FTM_GPIO_Pin GPIO_Pin_0
#define FTMx FTM0
static void FTM_Configuration (void)
{
	uint32_t delay = 400000;
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;
	FTM_PORT->PCR[FTM_GPIO_Pin] &= ~PORT_PCR_MUX_MASK;
	FTM_PORT->PCR[FTM_GPIO_Pin] |= PORT_PCR_MUX(3);
	FTMx->SC &=~ FTM_SC_CLKS_MASK;
	FTMx->SC &= ~FTM_SC_PS_MASK;
	FTMx->SC |= (FTM_SC_CLKS(1)| FTM_SC_PS(0));
	FTMx->MOD = 5000;						//计数器最大值
	FTMx->CNTIN = 0x0000u;				//计数器初始化时的计数值
	FTMx->CNT = 0x0000u;				//计数器开始的值
	while(delay--);
	//禁止写保护
	FTMx->MODE |= FTM_MODE_WPDIS_MASK;
  //禁止增强模式
	FTMx->MODE &= ~FTM_MODE_FTMEN_MASK;
  //禁止正交编码模式
	FTMx->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
  //边沿对齐
	FTMx->SC &= ~FTM_SC_CPWMS_MASK;
	FTMx->CONTROLS[5].CnSC = 0;
	FTMx->CONTROLS[5].CnSC |= (FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK);
	FTMx->COMBINE &= ~FTM_COMBINE_DECAPEN2_MASK;
	FTMx->COMBINE &= ~FTM_COMBINE_COMBINE2_MASK;	
	FTMx->CONTROLS[5].CnV = 2500;
}
static void GPIO_Configuration (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIOx = AD5453_SYNC;
	GPIO_InitStruct.GPIO_InitState = Bit_SET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = AD5453_SYNC_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_Init (&GPIO_InitStruct);

	GPIO_InitStruct.GPIOx = AD5453_SDIN;
	GPIO_InitStruct.GPIO_Pin = AD5453_SDIN_Pin;
	GPIO_Init (&GPIO_InitStruct);
	
// 	GPIO_InitStruct.GPIOx = AD5453_SCLK;
// 	GPIO_InitStruct.GPIO_Pin = AD5453_SCLK_Pin;
// 	GPIO_Init (&GPIO_InitStruct);
// 	GPIO_InitStruct.GPIOx = ML2035_SCK;
// 	GPIO_InitStruct.GPIO_Pin = ML2035_SCK_Pin;
// 	GPIO_Init (&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIOx = ML2035_LATI;
	GPIO_InitStruct.GPIO_Pin = ML2035_LATI_Pin;
	GPIO_Init (&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIOx = ML2035_SID;
	GPIO_InitStruct.GPIO_Pin = ML2035_SID_Pin;
	GPIO_Init (&GPIO_InitStruct);
	
	/* 涓轰简鏂逛究AD7687浣跨敤 */
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(1);
 	WV_SCK->PDDR |= (1<<WV_SCK_Pin);
// 	WV_SCK_PORT->PCR[WV_SCK_Pin]&= ~PORT_PCR_ODE_MASK;

	GPIO_SetBits (WV_SCK, WV_SCK_Pin);
	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(2);
	
// 	WV_SCK_PORT->PCR[WV_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
// 	WV_SCK_PORT->PCR[WV_SCK_Pin]|=PORT_PCR_MUX(2);
}

