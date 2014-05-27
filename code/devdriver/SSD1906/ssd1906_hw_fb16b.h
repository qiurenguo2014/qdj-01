/*========================================================================
#   FileName: ssd1906_hw_fb16b.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-24 10:08:03
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SSD1906_HW_FB16B_H
#define __SSD1906_HW_FB16B_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
typedef struct{
	volatile uint8_t data;
}LCD_REGISTER;
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define LCD_FLEX_BASE_ADDRESS (0X60000000)
#define LCD_FLEX_ADDRESS_MASK (0X1f0000)
#define LCD_FLEX_MEM_ADDRESS (0X60040000)
#define SCREEN_X_SIZE 320
#define SCREEN_Y_SIZE 240
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void SSD1906_Configurationg (void);
extern void SSD1906_OpenBackLight (void);
extern void SSD1906_CloseBackLight (void);
extern void SSD1906_openLed1 (void);
extern void SSD1906_openLed2 (void);
extern void SSD1906_closeLed1 (void);
extern void SSD1906_closeLed2 (void);
extern void SSD1906_openSpeak (void);
extern void SSD1906_closeSpeak (void);
extern uint8_t SSD1906_GetGpioStatus (void);
extern void SSD1906_UpdataScreen (uint16_t *pData);
extern void SSD1906_SetDot (int16_t X_coor, int16_t Y_coor, uint16_t color);
extern void SSD1906_SetBacklightLevel (uint8_t level);
extern uint8_t SSD1906_GetGpioStatus (void);
/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))
/*
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
#define GRAY6	            RGB565CONVERT(32,   32,     32)
*/

#define WHITE				0xFFFF
#define BLACK				0x0000	  
#define BLUE				0x001F  
#define BRED				0XF81F
#define GRED				0XFFE0
#define GBLUE				0X07FF
#define RED					0xF800
#define MAGENTA				0xF81F
#define GREEN				0x07E0
#define CYAN				0x7FFF
#define YELLOW				0xFFE0
#define BROWN				0XBC40 //
#define BRRED				0XFC07 //
#define GRAY				0X8430 //

#define DARKBLUE      		 0X01CF	//
#define LIGHTBLUE   	   	 0X7D7C	//
#define GRAYBLUE     	  	 0X5458 //

#define LIGHTGREEN			0X841F //
#define LIGHTGRAY			0XEF5B //
#define LGRAY				0XC618 //
#define LGRAYBLUE        	0XA651 //
#define LBBLUE           	0xB6FE //
#define ORINGE				0xFD00
// Development boards I/O definition

/******************************************************************************
* Overview: SSD1926 registers definitions.
*******************************************************************************/
#define REG_DISP_BUFFER_SIZE            0x01
#define REG_CONFIG_READBACK             0x02
#define REG_REVISION_CODE               0x03
#define REG_MEMCLK_CONFIG               0x04
#define REG_PCLK_CONFIG                 0x05
#define REG_LUT_BLUE_WRITE_DATA         0x08
#define REG_LUT_GREEN_WRITE_DATA        0x09
#define REG_LUT_RED_WRITE_DATA          0x0a
#define REG_LUT_WRITE_ADDR              0x0b
#define REG_LUT_BLUE_READ_DATA          0x0c
#define REG_LUT_GREEN_READ_DATA         0x0d
#define REG_LUT_RED_READ_DATA           0x0e
#define REG_LUT_READ_ADDR               0x0f
#define REG_PANEL_TYPE                  0x10
#define REG_MOD_RATE                    0x11
#define REG_HORIZ_TOTAL                 0x12
#define REG_HDP                         0x14
#define REG_HDP_START_POS0              0x16
#define REG_HDP_START_POS1              0x17
#define REG_VERT_TOTAL0                 0x18
#define REG_VERT_TOTAL1                 0x19
#define REG_VDP0                        0x1c
#define REG_VDP1                        0x1d
#define REG_VDP_START_POS0              0x1e
#define REG_VDP_START_POS1              0x1f
#define REG_HSYNC_PULSE_WIDTH           0x20
#define REG_HSYNC_PULSE_START_POS0      0x22
#define REG_HSYNC_PULSE_START_POS1      0x23
#define REG_VSYNC_PULSE_WIDTH           0x24
#define REG_VSYNC_PULSE_START_POS0      0x26
#define REG_VSYNC_PULSE_START_POS1      0x27
 
#define REG_FPFRAME_START_OFFSET0       0x30
#define REG_FPFRAME_START_OFFSET1       0x31
#define REG_FPFRAME_STOP_OFFSET0        0x34
#define REG_FPFRAME_STOP_OFFSET1        0x35
#define REG_HRTFT_SPECIAL_OUTPUT        0x38
#define REG_GPIO1_PULSE_START	        0x3c
#define REG_GPIO1_PULSE_STOP	        0x3e
#define REG_GPIO2_PULSE_DELAY 			0x40
#define REG_STN_COLOR_DEPTH				0x45
#define REG_DYN_DITHER_CONTROL			0x50

#define REG_DISPLAY_MODE                0x70
#define REG_SPECIAL_EFFECTS             0x71
#define REG_MAIN_WIN_DISP_START_ADDR0   0x74
#define REG_MAIN_WIN_DISP_START_ADDR1   0x75
#define REG_MAIN_WIN_DISP_START_ADDR2   0x76
#define REG_MAIN_WIN_ADDR_OFFSET0       0x78
#define REG_MAIN_WIN_ADDR_OFFSET1       0x79
#define REG_FLOAT_WIN_DISP_START_ADDR0  0x7c
#define REG_FLOAT_WIN_DISP_START_ADDR1  0x7d
#define REG_FLOAT_WIN_DISP_START_ADDR2  0x7e
#define REG_FLOAT_WIN_ADDR_OFFSET0      0x80
#define REG_FLOAT_WIN_ADDR_OFFSET1      0x81
#define REG_FLOAT_WIN_X_START_POS0      0x84
#define REG_FLOAT_WIN_X_START_POS1      0x85
#define REG_FLOAT_WIN_Y_START_POS0      0x88
#define REG_FLOAT_WIN_Y_START_POS1      0x89
#define REG_FLOAT_WIN_X_END_POS0        0x8c
#define REG_FLOAT_WIN_X_END_POS1        0x8d
#define REG_FLOAT_WIN_Y_END_POS0        0x90
#define REG_FLOAT_WIN_Y_END_POS1        0x91
#define REG_POWER_SAVE_CONFIG           0xa0
#define REG_SOFTWARE_RESET              0xa2
#define REG_SCRATCH_PAD0                0xa4
#define REG_SCRATCH_PAD1                0xa5
#define REG_GPIO_CONFIG0                0xa8
#define REG_GPIO_CONFIG1                0xa9
#define REG_GPIO_STATUS_CONTROL0        0xac
#define REG_GPIO_STATUS_CONTROL1        0xad
#define REG_PWM_CV_CLOCK_CONTROL        0xb0
#define REG_PWM_CV_CLOCK_CONFIG         0xb1
#define REG_CV_CLOCK_BURST_LENGTH       0xb2
#define REG_PWM_CLOCK_DUTY_CYCLE        0xb3

#define REG_CURSOR_FEATURE				0xc0
#define REG_CURSOR1_BLINK_TOTAL0		0xc4
#define REG_CURSOR1_BLINK_TOTAL1		0xc5
#define REG_CURSOR1_BLINK_ON0			0xc8
#define REG_CURSOR1_BLINK_ON1			0xc9
#define REG_CURSOR1_MEM_START0			0xcc
#define REG_CURSOR1_MEM_START1			0xcd
#define REG_CURSOR1_MEM_START2			0xce
#define REG_CURSOR1_POSX0				0xd0
#define REG_CURSOR1_POSX1				0xd1
#define REG_CURSOR1_POSY0				0xd4
#define REG_CURSOR1_POSY1				0xd5
#define REG_CURSOR1_HORIZ_SIZE			0xd8
#define REG_CURSOR1_VERT_SIZE			0xdc
#define REG_CURSOR1_COL_IND1_0			0xe0
#define REG_CURSOR1_COL_IND1_1			0xe1
#define REG_CURSOR1_COL_IND2_0			0xe4
#define REG_CURSOR1_COL_IND2_1			0xe5
#define REG_CURSOR1_COL_IND3_0			0xe8
#define REG_CURSOR1_COL_IND3_1			0xe9
#define REG_CURSOR2_BLINK_TOTAL0		0xec
#define REG_CURSOR2_BLINK_TOTAL1		0xed
#define REG_CURSOR2_BLINK_ON0			0xf0
#define REG_CURSOR2_BLINK_ON1			0xf1
#define REG_CURSOR2_MEM_START0			0xf4
#define REG_CURSOR2_MEM_START1			0xf5
#define REG_CURSOR2_MEM_START2			0xf6
#define REG_CURSOR2_POSX0				0xf8
#define REG_CURSOR2_POSX1				0xf9
#define REG_CURSOR2_POSY0				0xfc
#define REG_CURSOR2_POSY1				0xfd
#define REG_CURSOR2_HORIZ_SIZE			0x100
#define REG_CURSOR2_VERT_SIZE			0x104
#define REG_CURSOR2_COL_IND1_0			0x108
#define REG_CURSOR2_COL_IND1_1			0x109
#define REG_CURSOR2_COL_IND2_0			0x10c
#define REG_CURSOR2_COL_IND2_1			0x10d
#define REG_CURSOR2_COL_IND3_0			0x110
#define REG_CURSOR2_COL_IND3_1			0x111
#define REG_CIR							0x134

#endif 
/*********************************END OF FILE**********************************/


