/**********************************************************************
* @File    : 	SN74121.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.10
* @Brief   : 	µ¥ÎÈÌ¬´¥·¢Æ÷Çý¶¯
***********************************************************************/
#ifndef __SN74121_H_
#define __SN74121_H_

// Include
#include "../../common/DataType.h"
#include "../../bsp/fwlib/gpio.h"
#include "../../bsp/fwlib/UART.h"

// Pin Map
#define SN74121_ENA_PORT					PTE
#define SN74121_ENA_PIN						GPIO_Pin_26
#define SN74121_EXT_PORT					PTA
#define SN74121_EXT_PIN						GPIO_Pin_5
#define SN74121_EXT_IRQ						PORTA_IRQn

// Public
extern void SN74121_Init(void);
extern void SN74121_SetEnabled(void);
extern void SN74121_SetDisabled(void);

#endif
