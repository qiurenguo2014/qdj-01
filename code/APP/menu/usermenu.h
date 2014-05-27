/*************************************************************************
#    FileName: usermenu.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-10 10:52:37
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __USERMENU_H
#define __USERMENU_H
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "../../gui/gui_font.h"
#include "../../bsp/flash/memory.h"
/* Exported types ------------------------------------------------------*/
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
#define PARA_LIGHT_MASK 0X3
#define PARA_BEEP_MASK 0x4
#define PARA_LIGHTTIME 0XFFF<<3
#define UM_SET_SYSTEMPARA(l) MM_ProgramLWord (OFFSET_SYSTEM_PARA,l)
#define UM_GET_SYSTEMPARA MM_ReadLWord (OFFSET_SYSTEM_PARA)
#define UM_SET_SYSTEMLIGHTTIME(l) MM_ProgramLWord (OFFSET_SYSTEM_LT,l)
#define UM_GET_SYSTEMLIGHTTIME MM_ReadLWord (OFFSET_SYSTEM_LT)
#define UM_SET_BACKLIGHT(l) MM_ProgramLWord (OFFSET_BACKLIGHT,l);
#define UM_GET_BACKLIGHT MM_ReadLWord(OFFSET_BACKLIGHT)
#define UM_SET_LIGHTLEVEL(l) MM_ProgramLWord (OFFSET_LIGHTLEVEL,l);
#define UM_GET_LIGHTLEVEL MM_ReadLWord(OFFSET_LIGHTLEVEL)
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern void UM_Configuration (void);
extern void UM_Thread_Sample (uint8_t key_msg);
#endif 
/*********************************END OF FILE****************************/


