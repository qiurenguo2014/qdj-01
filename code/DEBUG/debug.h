/*========================================================================
#   FileName: debug.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-23 10:09:30
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H
#define __DEBUG_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define DEBUGALL 1
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void DB_Configuration (void);
extern void DB_printf (char *fmt, ...);
extern void DB_printf1 (char *fmt, ...);
#endif 
/*********************************END OF FILE**********************************/


