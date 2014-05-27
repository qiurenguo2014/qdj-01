/*========================================================================
#   FileName: wave.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-21 17:27:33
========================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVE_H
#define __WAVE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define FCLKIN 10000

#define AD5453_SYNC PTB
#define AD5453_SYNC_Pin GPIO_Pin_0
#define AD5453_SDIN PTA
#define AD5453_SDIN_Pin GPIO_Pin_16
#define AD5453_SCLK PTA
#define AD5453_SCLK_Pin GPIO_Pin_15
#define ML2035_LATI PTB
#define ML2035_LATI_Pin GPIO_Pin_10
#define ML2035_SID PTA
#define ML2035_SID_Pin GPIO_Pin_16
#define ML2035_SCK PTA
#define ML2035_SCK_Pin GPIO_Pin_15
#define WV_SCK PTA
#define WV_SCK_PORT PORTA
#define WV_SCK_Pin GPIO_Pin_15
#define WV_SET_ML2035_LATI GPIO_SetBits (ML2035_LATI, ML2035_LATI_Pin)
#define WV_RESET_ML2035_LATI GPIO_ResetBits (ML2035_LATI, ML2035_LATI_Pin)
#define WV_SET_ML2035_SID GPIO_SetBits (ML2035_SID, ML2035_SID_Pin)
#define WV_RESET_ML2035_SID GPIO_ResetBits (ML2035_SID, ML2035_SID_Pin)
#define WV_SET_ML2035_SCK GPIO_SetBits (ML2035_SCK, ML2035_SCK_Pin)
#define WV_RESET_ML2035_SCK GPIO_ResetBits (ML2035_SCK, ML2035_SCK_Pin)

#define WV_SET_AD5453_SYNC GPIO_SetBits (AD5453_SYNC, AD5453_SYNC_Pin)
#define WV_RESET_AD5453_SYNC GPIO_ResetBits (AD5453_SYNC, AD5453_SYNC_Pin)
#define WV_SET_AD5453_SDIN GPIO_SetBits (AD5453_SDIN, AD5453_SDIN_Pin)
#define WV_RESET_AD5453_SDIN GPIO_ResetBits (AD5453_SDIN, AD5453_SDIN_Pin)
#define WV_SET_AD5453_SCLK GPIO_SetBits (AD5453_SCLK, AD5453_SCLK_Pin)
#define WV_RESET_AD5453_SCLK GPIO_ResetBits (AD5453_SCLK, AD5453_SCLK_Pin)
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void WV_Configuration (void);
extern void WV_closeMl2035 (void);
extern void WV_SetFrequency (uint32_t hz);
extern void WV_SetAmplitude (uint32_t mV);
#endif 

/*********************************END OF FILE**********************************/


