/*========================================================================
#   FileName: systick.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-22 16:07:05
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTICK_H
#define __SYSTICK_H
/* Includes ------------------------------------------------------------------*/
#include <time.h>
/* Exported types ------------------------------------------------------------*/
#define JIFFIES_MAX 10
#define JIFFIES_WDOG 0		//WDOG 5s
#define SYSTICK_SWDOG (0X1<<0)
typedef struct{
	unsigned int status;
	long volatile jiffies[JIFFIES_MAX];
}SYSTICK_StructType;
extern SYSTICK_StructType Systick;
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern long volatile jiffies;
/* Exported functions prototypes ---------------------------------------------*/
extern void STK_Configuration (void);
extern void STK_delay10ms (long jif);
#endif 
/*********************************END OF FILE**********************************/


