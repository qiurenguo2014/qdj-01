/*========================================================================
#   FileName: ad7687.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-25 11:40:14
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AD7687_H
#define __AD7687_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "../../bsp/fwlib/gpio.h"
#include "../../bsp/fwlib/spi.h"
#include "../../bsp/fwlib/ftm.h"
#include "../../bsp/fwlib/pit.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define FTM_DUTY 5000		//0-10000  0%-100%
#define AD7687_SPI SPI0
#define AD7687_SIN PTA
#define AD7687_SIN_PORT PORTA
#define AD7687_SIN_Pin GPIO_Pin_17
#define AD7687_SIN_IRQ PORTA_IRQn
#define AD7687_SOUT PTA
#define AD7687_SOUT_PORT PORTA
#define AD7687_SOUT_Pin GPIO_Pin_16
#define AD7687_SCK PTA
#define AD7687_SCK_PORT PORTA
#define AD7687_SCK_Pin GPIO_Pin_15
#define AD7687_DATA     (SPI0_SCK_PA15_SOUT_PA16_SIN_PA17)
#define AD7687_CS       (SPI0_PCS0_PA14)
#define AD7687_CNV_PORT PORTB
#define AD7687_CNV		PTB
#define AD7687_CNV_Pin	GPIO_Pin_1
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void AD7687_Configuration (void);
extern void AD7687_SendWord (uint16_t data);
extern uint16_t AD7687_ReceiveWord(void);
extern void AD7687_GetConverData (uint16_t *ad);
extern void AD7687_StartConver (void);
extern void AD7687_ResetConverPin (void);
extern uint32_t AD7687_WaitConverOver (void);
extern void AD7687_StopConver (void);
#endif
/*********************************END OF FILE**********************************/


