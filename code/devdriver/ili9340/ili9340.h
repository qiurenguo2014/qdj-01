/*========================================================================
#   FileName: ili9430.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-12 13:54:53
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __ILI9430_H
#define __ILI9430_H

/* Includes ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------*/
//typedef struct{
//uint16_t X_MAX;
//uint16_t Y_MAX;
//uint16_t X_START;
//uint16_t Y_START;
//uint16_t X_END;
//uint16_t Y_END;
//uint16_t TextColor;
//uint16_t BackColor;
//}COLOR_DISPLAY;

//#define WINDOW_X_MAX    240
//#define WINDOW_Y_MAX    128
//#define WINDOW_X_START  40
//#define WINDOW_Y_STATR  56
//#define WINDOW_X_END   (WINDOW_X_START + WINDOW_X_MAX-1)
//#define WINDOW_Y_END   (WINDOW_Y_STATR + WINDOW_Y_MAX-1)
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
#define ILI_RESET_BITSET GPIO_SetBits (ILI_RESET, ILI_RESET_Pin);
#define ILI_RESET_BITRESET GPIO_ResetBits (ILI_RESET, ILI_RESET_Pin);
#define ILI_SPI SPI1
#define ILI_SIN PTE
#define ILI_SIN_PORT PORTE
#define ILI_SIN_Pin GPIO_Pin_3
#define ILI_SOUT PTE
#define ILI_SOUT_PORT PORTE
#define ILI_SOUT_Pin GPIO_Pin_1
#define ILI_SCK PTE
#define ILI_SCK_PORT PORTE
#define ILI_SCK_Pin GPIO_Pin_2
#define ILI_CS PTE
#define ILI_CS_PORT PORTE
#define ILI_CS_Pin GPIO_Pin_0
#define ILI_RESET PTD
#define ILI_RESET_PORT PORTD
#define ILI_RESET_Pin GPIO_Pin_7
/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/
extern void ILI_Configuration (void);
extern void ILI_Initial (void);
extern void ILI_ResetDevice (void);
extern void ILI_WriteData (uint16_t data);
extern void ILI_WriteCmd (uint16_t cmd);
extern void ILI_Read_1Byte (uint16_t cmd, uint32_t *data);
extern void ILI_Read_3Bytes (uint16_t cmd, uint32_t *data);

/* Ili9430 Register define */
#define ILI9430_SOFTWARE_RESET         0X01
#define ILI9430_ENTER_SLEEP_MODE       0X10
#define ILI9430_SLEEP_OUT              0X11
#define ILI9430_PARTIAL_MODE_ON        0X12
#define ILI9430_NORMAL_DISPLAY_MODE_ON 0X13
#define ILI9430_DISPLAY_INVERSION_OFF  0X20
#define ILI9430_DISPLAY_INVERSION_ON   0X21
#define ILI9430_GAMMA_SET              0X26
#define ILI9430_DISPLAY_OFF            0X28
#define ILI9430_DISPLAY_ON             0X29
#define ILI9430_COLUMN_ADDRESS_SET     0X2A
#define ILI9430_PAGE_ADDRESS_SET       0X2B
#define ILI9430_MEMORY_WRITE           0X2C
#define ILI9430_COLOR_SET              0X2D
#define ILI9430_MEMORY_READ            0X2E

/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
/*
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

#define BLACK               RGB565CONVERT(0,    0,      0)
#define BRIGHTBLUE          RGB565CONVERT(0,    0,      255)
#define BRIGHTGREEN         RGB565CONVERT(0,    255,    0)
#define BRIGHTCYAN          RGB565CONVERT(0,    255,    255)
#define BRIGHTRED           RGB565CONVERT(255,  0,      0)
#define BRIGHTMAGENTA       RGB565CONVERT(255,  0,      255)
#define BRIGHTYELLOW        RGB565CONVERT(255,  255,    0)
#define BLUE                RGB565CONVERT(0,    0,      0x38)
#define GREEN               RGB565CONVERT(0,    128,    0)
#define CYAN                RGB565CONVERT(0,    128,    128)
#define RED                 RGB565CONVERT(128,  0,      0)
#define MAGENTA             RGB565CONVERT(128,  0,      128)
#define BROWN               RGB565CONVERT(255,  128,    0)
#define LIGHTGRAY           RGB565CONVERT(128,  128,    128)
#define DARKGRAY            RGB565CONVERT(64,   64,     64)
#define LIGHTBLUE           RGB565CONVERT(128,  128,    255)
#define LIGHTGREEN          RGB565CONVERT(128,  255,    128)
#define LIGHTCYAN           RGB565CONVERT(128,  255,    255)
#define LIGHTRED            RGB565CONVERT(255,  128,    128)
#define LIGHTMAGENTA        RGB565CONVERT(255,  128,    255)
#define YELLOW              RGB565CONVERT(255,  255,    128)
#define WHITE               RGB565CONVERT(255,  255,    255)
                            
#define GRAY0       	    RGB565CONVERT(224,  224,    224)
#define GRAY1         	    RGB565CONVERT(192,  192,    192)   
#define GRAY2               RGB565CONVERT(160,  160,    160)   
#define GRAY3               RGB565CONVERT(128,  128,    128)
#define GRAY4               RGB565CONVERT(96,   96,     96)
#define GRAY5               RGB565CONVERT(64,   64,     64)
#define GRAY6	            RGB565CONVERT(32,   32,     32)*/
#endif 
/*********************************END OF FILE****************************/


