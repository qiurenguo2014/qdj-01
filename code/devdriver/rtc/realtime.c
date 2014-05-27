/*====================================================================
#   FileName: realtime.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-04 09:40:30
=====================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdio.h>
#include "realtime.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
RTC_StructType RTC_Struct;
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
/* Functions --------------------------------------------------------*/
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RTC_Init (void)
{
	RTC_Configuration ();
	//RTC_TimeResume ();
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RTC_PrintfDataTime (void)
{
	printf("%2d-%2d-%2d    %2d:%2d:%2d\r\n",RTC_Struct.tm_year,
			RTC_Struct.tm_mon,RTC_Struct.tm_mday,
			RTC_Struct.tm_hour,RTC_Struct.tm_min,
			RTC_Struct.tm_sec);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void RTC_GetTime (void)
{
	RTC_GetDataTime (&RTC_Struct);
}
/*********************************END OF FILE**********************************/


