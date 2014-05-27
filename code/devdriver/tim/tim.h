/*========================================================================
#   FileName: tim.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-22 14:03:04
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifndef __TIM_C
#define __TIM_C extern
#endif
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void PIT_Configuration (void);
extern void PIT_EnableIRQ (void);
extern void PIT_DisableIRQ (void);
extern void PIT_StartTim (void);
extern void PIT_StopTim (void);
#endif 
/*********************************END OF FILE**********************************/

