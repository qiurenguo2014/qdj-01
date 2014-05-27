/*========================================================================
#   FileName: memory.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-30 11:21:58
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEMORY_H
#define __MEMORY_H
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define BASE_ADDR				((uint32_t)0x0000000UL)
#define END_ADDR                (BASE_ADDR + 512 * 1024UL-1)
#define FLASH_SIZE              (512*1024UL)
#define FLASH_PAGE_SIZE         2048UL
#define FLASH_CODE_SIZE			(FLASH_PAGE_SIZE*70)
#define FLASH_START_ADDR		(BASE_ADDR + FLASH_CODE_SIZE)
#define FLASH_PAGGS(flashsize, pg_size)   ((flashsize) / (pg_size) + (((flashsize) % (pg_size) >0)?1:0) )
#define OFFSET_SYSTEM (FLASH_PAGE_SIZE*128-4-FLASH_START_ADDR)
#define OFFSET_SIGCHN_RZ_ZERO (0*4)
#define OFFSET_SIGCHN_RF_ZERO (1*4)
#define OFFSET_SIGCHN_CC_ZERO (2*4)
#define OFFSET_SIGCHN_VV_ZERO (3*4)
#define OFFSET_SIGCHN_GEZ_ERR (8*4)
#define OFFSET_SIGCHN_GEF_ERR (9*4)
#define OFFSET_SIGCHN_RZ_ERR (10*4)
#define OFFSET_SIGCHN_RF_ERR (11*4)
#define OFFSET_SIGCHN_CP_ERR (12*4)
#define OFFSET_BACKLIGHT (13*4)
#define OFFSET_LIGHTLEVEL (14*4)
#define OFFSET_SIGCHN_CR_ERR (15*4)
#define OFFSET_SIGCHN_RR_ERR (16*4)
#define OFFSET_SYSTEM_PARA (17*4)
#define OFFSET_SYSTEM_LT (18*4)
#define OFFSET_CAP1X_ERR (19*4)
#define OFFSET_CAP2X_ERR (20*4)
#define OFFSET_CAP3X_ERR (21*4)
#define OFFSET_SERIES_CAP1x (22*4)
#define OFFSET_SERIES_CAP2x (23*4)
#define OFFSET_SERIES_CAP3x (24*4)
#define SYSPARAM_INIT (0x1<<1)
#define SYSTIME_INIT (0x1<<1)
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void MM_Configuration (void);
extern uint32_t MM_ProgramLWord (uint32_t offset, uint32_t data);
extern uint32_t MM_ReadLWord (uint32_t offset);
extern void MM_SetSysFlag (uint32_t sysparam);
extern void MM_ClrSysFlag (uint32_t sysparam);
extern void MM_ResumeSysflag (void);
#endif 
/*********************************END OF FILE**********************************/


