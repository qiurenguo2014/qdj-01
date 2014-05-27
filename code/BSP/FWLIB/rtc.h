/*========================================================================
#   FileName: rtc.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-05 09:19:23
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __RTC_H
#define __RTC_H
#ifdef __cplusplus
 extern "C" {
#endif
#ifdef __cplusplus
}
#endif
/* Includes ------------------------------------------------------------*/
#include "sys.h"
/* Exported types ------------------------------------------------------*/
typedef struct 
{
    int tm_sec;   /* seconds after the minute, 0 to 60
                     (0 - 60 allows for the occasional leap second) */
    int tm_min;   /* minutes after the hour, 0 to 59 */
    int tm_hour;  /* hours since midnight, 0 to 23 */
    int tm_mday;  /* day of the month, 1 to 31 */
    int tm_mon;   /* months since January, 0 to 11 */
    int tm_year;  /* years since 1900 */
    int tm_wday;  /* days since Sunday, 0 to 6 */
    int tm_yday;  /* days since January 1, 0 to 365 */
    int tm_isdst; /* Daylight Savings Time flag */
}RTC_StructType;
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
//RTC ÷–∂œ‘¥
#define RTC_IT_TAF    (uint16_t)(0)
#define RTC_IT_TOF    (uint16_t)(1)
#define RTC_IT_TIF    (uint16_t)(2)
#define IS_RTC_IT(IT)  (((IT) == RTC_IT_TAF) || \
                        ((IT) == RTC_IT_TOF) || \
                        ((IT) == RTC_IT_TIF))
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern void RTC_Configuration (void);
extern void RTC_TimeResume (void);
extern unsigned int RTC_mktimev(RTC_StructType *tm);
extern void RTC_CorrectDay (RTC_StructType *time);
extern void RTC_to_tm (unsigned int tim, RTC_StructType *tm);
extern void RTC_SetCounter (uint32_t CounterValue);
extern void RTC_GetDataTime (RTC_StructType *tm);
#endif 
/*********************************END OF FILE****************************/
