/*========================================================================
#   FileName: BSP.C
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-18 15:50:31
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include "rtc.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

/* Variables --------------------------------------------------------*/
static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* Functions prototypes ---------------------------------------------*/
static void _RTC_EnterConfigMode (void);
static void _RTC_ExitConfigMode (void);
uint32_t RTC_GetCounter (void);
void RTC_SetCounter (uint32_t CounterValue);
uint32_t RTC_GetCounter (void);
/* Functions --------------------------------------------------------*/

void RTC_Configuration (void)
{
	unsigned int i;
	RTC_StructType tm;
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
	if (RTC->SR & RTC_SR_TCE_MASK){
		//printf("RTC_Config ok!\r\n");
	}else{
		RTC->SR &= ~RTC_SR_TCE_MASK;
		RTC->CR = RTC_CR_SWR_MASK;
		RTC->CR &= ~RTC_CR_SWR_MASK;		
		RTC->CR |= RTC_CR_OSCE_MASK;
		for(i=0;i<0x600000;i++);		
		RTC->TCR = RTC_TCR_CIR(0)|RTC_TCR_TCR(0);
		RTC->TSR = 0;
		RTC->TAR = 0;
		RTC->SR |= RTC_SR_TCE_MASK;
		RTC_TimeResume ();
		//printf ("RTC_Config init\r\n");
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RTC_GetDataTime (RTC_StructType *tm)
{
	unsigned int timecounter;
	timecounter = RTC_GetCounter ();
	RTC_to_tm (timecounter,tm);
//	printf("get timecounter:%u\r\n",timecounter);
}
/**
  * @brief  set time is program data and time.
  * @param  None
  * @retval None
  */
void RTC_TimeResume (void)
{
	char *buf;
	uint32_t timecounter;
	RTC_StructType rtc;
	buf = __DATE__;
	rtc.tm_year = (buf[7]-48)*1000;
	rtc.tm_year += (buf[8]-48)*100;
	rtc.tm_year += (buf[9]-48)*10;
	rtc.tm_year += (buf[10]-48);
	printf("tm_year:%u\r\n",rtc.tm_year);
	if (buf[0] == 'J'){
		if (buf[1] == 'a'){
			if (buf[2] == 'n'){
				rtc.tm_mon = 1;
			}
		}
	}
	if (buf[0] == 'F'){
		if (buf[1] == 'e'){
			if (buf[2] == 'b'){
				rtc.tm_mon = 2;
			}
		}
	}
	if (buf[0] == 'M'){
		if (buf[1] == 'a'){
			if (buf[2] == 'r'){
				rtc.tm_mon = 3;
			}
		}
	}
	if (buf[0] == 'A'){
		if (buf[1] == 'p'){
			if (buf[2] == 'r'){
				rtc.tm_mon = 4;
			}
		}
	}
	if (buf[0] == 'M'){
		if (buf[1] == 'a'){
			if (buf[2] == 'y'){
				rtc.tm_mon = 5;
			}
		}
	}
	if (buf[0] == 'J'){
		if (buf[1] == 'u'){
			if (buf[2] == 'n'){
				rtc.tm_mon = 6;
			}
		}
	}
	if (buf[0] == 'J'){
		if (buf[1] == 'u'){
			if (buf[2] == 'l'){
				rtc.tm_mon = 7;
			}
		}
	}
	if (buf[0] == 'A'){
		if (buf[1] == 'u'){
			if (buf[2] == 'g'){
				rtc.tm_mon = 8;
			}
		}
	}
	if (buf[0] == 'S'){
		if (buf[1] == 'e'){
			if (buf[2] == 'p'){
				rtc.tm_mon = 9;
			}
		}
	}
	if (buf[0] == 'O'){
		if (buf[1] == 'c'){
			if (buf[2] == 't'){
				rtc.tm_mon = 10;
			}
		}
	}
	if (buf[0] == 'N'){
		if (buf[1] == 'o'){
			if (buf[2] == 'v'){
				rtc.tm_mon = 11;
			}
		}
	}
	if (buf[0] == 'D'){
		if (buf[1] == 'e'){
			if (buf[2] == 'c'){
				rtc.tm_mon = 12;
			}
		}
	}
	printf("tm_mon:%u\r\n",rtc.tm_mon);
	if (buf[4] == ' '){
		rtc.tm_mday = (buf[5]-48);
	}else{
		rtc.tm_mday = (buf[4]-48)*10;
		rtc.tm_mday += (buf[5]-48);
	}
	printf("tm_mday:%u\r\n",rtc.tm_mday);
	buf = __TIME__;
	rtc.tm_hour = (buf[0]-48)*10;
	rtc.tm_hour += (buf[1]-48);
	rtc.tm_min = (buf[3]-48)*10;
	rtc.tm_min += (buf[4]-48);
	rtc.tm_sec = (buf[6]-48)*10;
	rtc.tm_sec += (buf[7]-48);
	printf("h:%d,m:%d,s:%d\r\n",rtc.tm_hour,rtc.tm_min,rtc.tm_sec);
	RTC_CorrectDay (&rtc);
	timecounter = RTC_mktimev (&rtc);
	RTC_to_tm (timecounter, &rtc);
	RTC_SetCounter (timecounter);	
}
/**
  * @brief  矫正自己设置的时间日期.
  * @param  None
  * @retval None
  */
void RTC_CorrectDay (RTC_StructType *time)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
	lastYear=time->tm_year-1;
	/*
	 * Number of leap corrections to apply up to end of last year
	 */
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;
	/*
	 * This year is a leap year if it is divisible by 4 except when it is
	 * divisible by 100 unless it is divisible by 400
	 *
	 * e.g. 1904 was a leap year, 1900 was not, 1996 is, and 2000 will be
	 */
	if((time->tm_year%4==0) &&
	   ((time->tm_year%100!=0) || (time->tm_year%400==0)) &&
	   (time->tm_mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[time->tm_mon-1] + time->tm_mday;
	time->tm_wday=day%7;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
unsigned int RTC_mktimev(RTC_StructType *tm)
{
	if (0 >= ((int)tm->tm_mon -= 2)) {//((int)tm->tm_mon -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->tm_mon += 12;		/* Puts Feb last since it has leap day */
		tm->tm_year -= 1;
	}
	return (((
		(uint32_t) (tm->tm_year/4 - tm->tm_year/100 + tm->tm_year/400 + 367*tm->tm_mon/12 + tm->tm_mday) +
			tm->tm_year*365 - 719499ul
	    )*24 + tm->tm_hour /* now have hours */
	  )*60 + tm->tm_min /* now have minutes */
	)*60 + tm->tm_sec; /* finally seconds */
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RTC_to_tm(unsigned int tim, RTC_StructType *tm)
{
	uint32_t i;
	long hms,day;
	day = tim / SECDAY;
	hms = tim % SECDAY;
	/* Hours, minutes, seconds are easy */
	tm->tm_hour = hms / 3600;
	tm->tm_min = (hms % 3600) / 60;
	tm->tm_sec = (hms % 3600) % 60;
	/* Number of years in days */
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->tm_year = i;
	/* Number of months in days left */
	if (leapyear(tm->tm_year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->tm_mon = i+2;
	/* Days are what is left over (+1) from all that.*/
	tm->tm_mday = day + 1;
	/*
	 * Determine the day of week
	 */
	RTC_mktimev(tm);
}

/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _RTC_EnterConfigMode (void)
{
	RTC->SR &= ~RTC_SR_TCE_MASK;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _RTC_ExitConfigMode (void)
{
	RTC->SR |= RTC_SR_TCE_MASK;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t RTC_GetCounter (void)
{
	return (RTC->TSR);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void RTC_SetCounter (uint32_t CounterValue)
{
	_RTC_EnterConfigMode ();
	RTC->TSR = CounterValue;
	_RTC_ExitConfigMode ();
}
//中断配置
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState)
{
	//检查参数
	assert_param(IS_RTC_IT(RTC_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));	
// 	switch(RTC_IT)
// 	{
// 		case RTC_IT_TAF:
// 			(ENABLE == NewState)?(RTC->IER |= RTC_IER_TAIE_MASK):(RTC->IER &= ~RTC_IER_TAIE_MASK);
// 			break;
// 		case RTC_IT_TOF:
// 			(ENABLE == NewState)?(RTC->IER |= RTC_IER_TOIE_MASK):(RTC->IER &= ~RTC_IER_TOIE_MASK);
// 			break;
// 		case RTC_IT_TIF:
// 			(ENABLE == NewState)?(RTC->IER |= RTC_IER_TIIE_MASK):(RTC->IER &= ~RTC_IER_TIIE_MASK);
// 			break;
// 		default:break;
// 	}
	
}
//获得中断状态
ITStatus RTC_GetITStatus(uint16_t RTC_IT)
{
	ITStatus retval;
	//检查参数
	assert_param(IS_RTC_IT(RTC_IT));
	
	switch(RTC_IT)
	{
		case RTC_IT_TAF:
 		 (RTC->SR & RTC_SR_TAF_MASK)?(retval = SET):(retval = RESET);
			break;
		case RTC_IT_TOF:
 		 (RTC->SR & RTC_SR_TOF_MASK)?(retval = SET):(retval = RESET);
			break;
		case RTC_IT_TIF:
 		 (RTC->SR & RTC_SR_TIF_MASK)?(retval = SET):(retval = RESET);
			break;
		default:break;
	}
	return retval;
}
/***********************************************************************************************
 功能：判断是否是闰年
 形参：年份
 返回：1 闰年 0 平年
 详解：判断是否是闰年函数
			月份   1  2  3  4  5  6  7  8  9  10 11 12
			闰年   31 29 31 30 31 30 31 31 30 31 30 31
			非闰年 31 28 31 30 31 30 31 31 30 31 30 31
************************************************************************************************/
static uint8_t RTC_IsLeapYear(uint16_t year)
{
	if(year % 4 == 0) //必须能被4整除
	{ 
		if(year % 100 == 0) 
		{ 
			if(year % 400 == 0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}
