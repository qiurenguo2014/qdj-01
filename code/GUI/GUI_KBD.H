/*************************************************************************
#    FileName: gui_kbd.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-26 16:41:29
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __GUI_KBD_H
#define __GUI_KBD_H

/* Includes ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------*/
#define KEY_COUNTER 3
typedef enum{
	KBD_UP,KBD_DOWN,KBD_ENTER,KBD_BACK,KBD_POWER
}KBD_STATUS;
typedef struct{
	volatile u32 msg;
	volatile u32 msg_u;
}KBD_StructType;
extern KBD_StructType KBD_Struct;
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
#define KBD_MSG_UP 0X1
#define KBD_MSG_DOWN 0X02
#define KBD_MSG_ENTER 0X04
#define KBD_UP_BIT 0X10
#define KBD_DOWN_BIT 0X08
#define KBD_ENTER_BIT 0X20
#define KBD_BACK_BIT 0X04
#define KBD_POWER_BIT
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern void KBD_Init (void);
extern void KBD_ReadKey_u (void);
#endif 
/*********************************END OF FILE****************************/


