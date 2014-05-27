/*========================================================================
#   FileName: adg.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-21 11:06:21
========================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADG_H
#define __ADG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum{
	ADG_CHANNEL_1,
	ADG_CHANNEL_2,
	ADG_CHANNEL_3,
	ADG_CHANNEL_4,
}ADG_CHANNEL_n;
typedef struct{
	ADG_CHANNEL_n CHN;
}ADG_StructType;
extern ADG_StructType ADG_Struct;
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define ADG_EARTHRES_CHANNEL ADG_CHANNEL_1
#define ADG_CAP_CHANNEL1x ADG_CHANNEL_2//Ò»µµ 1x
#define ADG_CAP_CHANNEL2x ADG_CHANNEL_3//¶þµµ 51x
#define ADG_CAP_CHANNEL3x ADG_CHANNEL_4//Èýµµ 501x
#define ADG_A0 PTA
#define ADG_A0_Pin GPIO_Pin_19
#define ADG_A1 PTA
#define ADG_A1_Pin GPIO_Pin_4

/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void ADG_Configuration (void);
extern void ADG_SetChannel (ADG_CHANNEL_n chn);
#endif 

/*********************************END OF FILE**********************************/

