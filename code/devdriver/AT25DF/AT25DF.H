/*========================================================================
#   FileName: AT25DF.H
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-07 08:53:54
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __AT25DF_H
#define __AT25DF_H

/* Includes ------------------------------------------------------------*/
#include "../../includes.h"
/* Exported types ------------------------------------------------------*/
typedef enum{
	RDY_BSY,	//Ready/Busy Status
	WEL,		//Write Enable Latch Status
	SWP1,		//Software Protection Status
	SWP2,
	WPP,		//Write Protect (WP) Pin Status
	EPE,		//Erase/Program Error
	RES,		//Reserved for future use
	SPRL,		//Sector Protection Registers Locked
}SR_BYTE1;		//SR[0]
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
#define AT25_SPI SPI1
#define AT25_SIN PTE
#define AT25_SIN_PORT PORTE
#define AT25_SIN_Pin GPIO_Pin_3
#define AT25_SOUT PTE
#define AT25_SOUT_PORT PORTE
#define AT25_SOUT_Pin GPIO_Pin_1
#define AT25_SCK PTE
#define AT25_SCK_PORT PORTE
#define AT25_SCK_Pin GPIO_Pin_2
#define AT25_CS PTE
#define AT25_CS_PORT PORTE
#define AT25_CS_Pin GPIO_Pin_6
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern void AT_Configuration (void);
extern u8 AT_SendReceiveCont (u8 byte);
extern u8 AT_SendReceiveNcont (u8 byte);
extern void AT_ReadArrayByte (u32 addr, u8 *data);
extern void AT_ReadMng_DrvID (u8 *data);
extern void AT_ReadStatusRegister (u8 *data);
extern void AT_ProgramByte (u32 addr, u8 data);
extern void AT_ProgramPage (u32 addr, u8 *data, u16 counter);
extern void AT_WriteEnable (void);
extern void AT_SetStatusRegByte1 (u8 data);
extern void AT_BlockErase4K (u32 addr);
extern void AT_BlockErase32K (u32 addr);
extern void AT_BlockErase64K (u32 addr);
extern void AT_ChipErase (void);
#endif
/*********************************END OF FILE****************************/


