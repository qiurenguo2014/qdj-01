
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

/* Includes ------------------------------------------------------------------*/
#include "fwlib/sys.h"
#include "uart\bsp_uart.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void BSP_Configuration (void);
extern void BSP_PowerInit (void);
extern void BSP_PowerDown (void);
extern void BSP_PowerUp (void);
#endif 

/*********************************END OF FILE**********************************/

