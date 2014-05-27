/*========================================================================
#   FileName: realtime.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-04 09:40:40
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REALTIME_H
#define __REALTIME_H
/* Includes ------------------------------------------------------------------*/
#include "../../bsp/fwlib/rtc.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
extern RTC_StructType RTC_Struct;
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void RTC_Init (void);
extern void RTC_PrintfDataTime (void);
extern void RTC_GetTime (void);
#endif 
/*********************************END OF FILE**********************************/


