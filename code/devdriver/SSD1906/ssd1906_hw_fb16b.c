/*========================================================================
#   FileName: ssd1906_hw_fb16b.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-24 10:07:18
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdint.h>
#include "..\..\bsp\fwlib\gpio.h"
#include "ssd1906_hw_fb16b.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
/* Variables --------------------------------------------------------*/
LCD_REGISTER ssd1906_reg[0x135];
volatile unsigned short *pSSD1906DAT = (volatile unsigned short *)LCD_FLEX_MEM_ADDRESS;
/* Functions prototypes ---------------------------------------------*/
static void _HW_Init_Flexbus_16b (void);
static void _SSD1906_RegisterDefault (void);
static void _SSD1906_Delay (uint32_t time);
static void _SSD1906_ClearScreen (void);
void SSD1906_RegisterInit (void);
void SSD1906_SetRegister (uint16_t addr, uint8_t data);
uint8_t SSD1906_GetRegister (uint16_t addr);
/* Functions --------------------------------------------------------*/
/**
  * @brief  Generic #2,GPIO OUT,little endian,wait# is active low,clki:blck 1:1.
  * @param  None
  * @retval None
  */
void SSD1906_Configurationg (void)
{
	_HW_Init_Flexbus_16b ();
	SSD1906_RegisterInit ();
	SSD1906_closeLed1 ();
	SSD1906_closeLed2 ();
	SSD1906_closeSpeak ();
	SSD1906_CloseBackLight ();
	_SSD1906_ClearScreen();
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_RegisterInit (void)
{
	_SSD1906_RegisterDefault ();
	/* command initial */
	SSD1906_SetRegister (REG_CIR, 0);
	/* reset driver */
	SSD1906_SetRegister (REG_SOFTWARE_RESET, 0X01);
	/* enable input */
	SSD1906_SetRegister (REG_GPIO_CONFIG1, 0X80);
	/* bit:1(output) 0(input) */
	SSD1906_SetRegister (REG_GPIO_CONFIG0, 0X43);
	/* set gpio status bit:1(high level) 0(low level) */
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, 0x3f);
	/* mem clk bclk:mclk=1:1 */
	SSD1906_SetRegister (REG_MEMCLK_CONFIG, 0x00);
	/* PCLKS:PCLKF=3:1  PCLKS(CLKI) */
	SSD1906_SetRegister (REG_PCLK_CONFIG, 0X22);
	_SSD1906_Delay (100);	
	
	SSD1906_SetRegister (REG_PWM_CV_CLOCK_CONTROL, 0X42);
	/* set pannel type color lcd 18bit tft */
	SSD1906_SetRegister (REG_PANEL_TYPE, 0x61);//0x10h//0x61
	/* mod rate(?) */
	SSD1906_SetRegister (REG_MOD_RATE, 0X01);//0x11h//0x01
	/* horiz in number of pixels = (bit[6:0]+1)*8=320 */
	SSD1906_SetRegister (REG_HORIZ_TOTAL, 0x27);
	/* 240 */
	SSD1906_SetRegister (REG_HDP, 0X1D);
	
	SSD1906_SetRegister (REG_HDP_START_POS0, 0X4A);//0x16h
	SSD1906_SetRegister (REG_VERT_TOTAL0, 0X5F);//0x18h
	SSD1906_SetRegister (REG_VERT_TOTAL1, 0x01);
	SSD1906_SetRegister (REG_VDP0, 0X3F);
	SSD1906_SetRegister (REG_VDP1, 0X01);
	SSD1906_SetRegister (REG_VDP_START_POS0, 0x12);
	SSD1906_SetRegister (REG_VDP_START_POS1, 0);
	/* pclk width  */
	SSD1906_SetRegister (REG_HSYNC_PULSE_WIDTH, 0x10);//0x20h
	SSD1906_SetRegister (REG_HSYNC_PULSE_START_POS0, 0x05);
	SSD1906_SetRegister (REG_HSYNC_PULSE_START_POS1, 0);
	SSD1906_SetRegister (REG_VSYNC_PULSE_WIDTH, 0);
	SSD1906_SetRegister (REG_VSYNC_PULSE_START_POS0, 0);
	SSD1906_SetRegister (REG_VSYNC_PULSE_START_POS1, 0);
	
	SSD1906_SetRegister (REG_FPFRAME_START_OFFSET0, 0);//0x30h
	SSD1906_SetRegister (REG_FPFRAME_START_OFFSET1, 0);
	SSD1906_SetRegister (REG_FPFRAME_STOP_OFFSET0, 0);
	SSD1906_SetRegister (REG_FPFRAME_STOP_OFFSET1, 0);
	SSD1906_SetRegister (REG_DISPLAY_MODE, 0X04);//0x70h
	SSD1906_SetRegister (REG_SPECIAL_EFFECTS, 0X13);//0x71//0x11
	
	SSD1906_SetRegister (REG_MAIN_WIN_DISP_START_ADDR0, 0X60);//0x9f
	SSD1906_SetRegister (REG_MAIN_WIN_DISP_START_ADDR1, 0x95);
	SSD1906_SetRegister (REG_MAIN_WIN_DISP_START_ADDR2, 0);
	SSD1906_SetRegister (REG_MAIN_WIN_ADDR_OFFSET0, 0xa0);//0xa0
	SSD1906_SetRegister (REG_MAIN_WIN_ADDR_OFFSET1, 0);	
	/* enable display, config power */
	SSD1906_SetRegister (REG_POWER_SAVE_CONFIG,0);
	_SSD1906_Delay (100);
	_SSD1906_ClearScreen ();
}
/**
  * @brief  .
  ***********************
  *	(x0y0)(x1y0)		*
  *					    *
  *				        *
  ***********************
  * @param  None
  * @retval None
  */
void SSD1906_SetDot (int16_t X_coor, int16_t Y_coor, uint16_t color)
{
	uint32_t offset;
	if (X_coor < SCREEN_X_SIZE && Y_coor < SCREEN_Y_SIZE){
		offset = Y_coor*SCREEN_X_SIZE+X_coor;
		offset = 76799-offset;
		pSSD1906DAT[offset] = color;
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_UpdataScreen (uint16_t *pData)
{
	register uint32_t i;
	i = 0;
	while (i<76800){
		pSSD1906DAT[i] = pData[i];
		i++;
	}
}
/**
  * @brief  GPIO2~GPIO5 (&0X3C).
  * @param  None
  * @retval None
  */
uint8_t SSD1906_GetGpioStatus (void)
{
	return SSD1906_GetRegister (REG_GPIO_STATUS_CONTROL0);
}
/**
  * @brief  open .
  * @param  None
  * @retval None
  */
void SSD1906_openLed1 (void)
{
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data |= 0x3b;
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data &= 0xfe;
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data);
}
/**
  * @brief  open .
  * @param  None
  * @retval None
  */
void SSD1906_openLed2 (void)
{
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data |= 0x3b;
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data &= 0xfd;
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_closeLed1 (void)
{
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data |= 0x3d;
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_closeLed2 (void)
{
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data |= 0x3e;
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_openSpeak (void)
{
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data |= 0x7c;
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, 
			ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_closeSpeak (void)
{
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data |= 0x3c;
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data &= 0xbf;
	SSD1906_SetRegister (REG_GPIO_STATUS_CONTROL0, 
			ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_OpenBackLight (void)
{
	SSD1906_SetRegister (REG_PWM_CV_CLOCK_CONTROL, 0X80);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_CloseBackLight (void)
{
	SSD1906_SetRegister (REG_PWM_CV_CLOCK_CONTROL, 0X0);
}
/**
  * @brief  set the screen to white.
  * @param  None
  * @retval None
  */
void _SSD1906_ClearScreen (void)
{
	register uint32_t i;	
	i = 76800;
	while (i--){
		pSSD1906DAT[i] = BLACK;
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_SetBacklightLevel (uint8_t duty)
{
	SSD1906_SetRegister (REG_PWM_CV_CLOCK_CONTROL, 0X10);
	SSD1906_SetRegister (REG_PWM_CV_CLOCK_CONFIG, 0X40);
	/* set pwm (light) */
	SSD1906_SetRegister (REG_PWM_CLOCK_DUTY_CYCLE, duty);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SSD1906_WriteMemData (uint16_t addr, uint16_t data)
{
	*(volatile uint16_t*)(LCD_FLEX_MEM_ADDRESS+addr) = data;
}
/**
  * @brief  MSB D15--------------D0 LSB.
  *				2N+1		2N
  * @param  None
  * @retval None
  */
void SSD1906_SetRegister (uint16_t addr, uint8_t data)
{
	uint16_t c_addr,c_data;
	c_addr = addr>>1;
	ssd1906_reg[addr].data = data;
	c_data = ssd1906_reg[c_addr*2+1].data;
	c_data <<= 8;
	c_data |= ssd1906_reg[c_addr*2].data;
	c_addr = addr&0xfffe;
	*(volatile uint16_t *)(LCD_FLEX_BASE_ADDRESS+c_addr) = c_data;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint8_t SSD1906_GetRegister (uint16_t addr)
{
	uint16_t c_addr,c_data;
	c_addr = addr&0xfffe;
	c_data = *(volatile uint16_t *)(LCD_FLEX_BASE_ADDRESS+c_addr);
	c_addr = addr >> 1;
	ssd1906_reg[c_addr*2+1].data = c_data>>8;
	ssd1906_reg[c_addr*2].data = (uint8_t)c_data;
	return ssd1906_reg[addr].data;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _SSD1906_RegisterDefault (void)
{
	ssd1906_reg[REG_DISP_BUFFER_SIZE].data = 0x40;
	ssd1906_reg[REG_CONFIG_READBACK].data = 0x04;
	ssd1906_reg[REG_REVISION_CODE].data = 0x78;
	ssd1906_reg[REG_MEMCLK_CONFIG].data = 0;
	ssd1906_reg[REG_PCLK_CONFIG].data = 0;
	ssd1906_reg[REG_LUT_BLUE_WRITE_DATA].data = 0;
	ssd1906_reg[REG_LUT_GREEN_WRITE_DATA].data = 0;
	ssd1906_reg[REG_LUT_RED_WRITE_DATA].data = 0;
	ssd1906_reg[REG_LUT_WRITE_ADDR].data = 0;
	ssd1906_reg[REG_LUT_BLUE_READ_DATA].data = 0;
	ssd1906_reg[REG_LUT_GREEN_READ_DATA].data = 0;
	ssd1906_reg[REG_LUT_RED_READ_DATA].data = 0;
	ssd1906_reg[REG_LUT_READ_ADDR].data = 0;
	ssd1906_reg[REG_PANEL_TYPE].data = 0;
	ssd1906_reg[REG_MOD_RATE].data = 0;
	ssd1906_reg[REG_HORIZ_TOTAL].data = 0;
	ssd1906_reg[REG_HDP].data = 0;
	ssd1906_reg[REG_HDP_START_POS0].data = 0;
	ssd1906_reg[REG_HDP_START_POS1].data = 0;
	ssd1906_reg[REG_VERT_TOTAL0].data = 0;
	ssd1906_reg[REG_VERT_TOTAL1].data = 0;
	ssd1906_reg[REG_VDP0].data = 0;
	ssd1906_reg[REG_VDP1].data = 0;
	ssd1906_reg[REG_VDP_START_POS0].data = 0;
	ssd1906_reg[REG_VDP_START_POS1].data = 0;
	ssd1906_reg[REG_HSYNC_PULSE_WIDTH].data = 0;
	ssd1906_reg[REG_HSYNC_PULSE_START_POS0].data = 0;
	ssd1906_reg[REG_HSYNC_PULSE_START_POS1].data = 0;
	ssd1906_reg[REG_VSYNC_PULSE_WIDTH].data = 0;
	ssd1906_reg[REG_VSYNC_PULSE_START_POS0].data = 0;
	ssd1906_reg[REG_VSYNC_PULSE_START_POS1].data = 0;
 
	ssd1906_reg[REG_FPFRAME_START_OFFSET0].data = 0;
	ssd1906_reg[REG_FPFRAME_START_OFFSET1].data = 0;
	ssd1906_reg[REG_FPFRAME_STOP_OFFSET0].data = 0;
	ssd1906_reg[REG_FPFRAME_STOP_OFFSET1].data = 0;
	ssd1906_reg[REG_HRTFT_SPECIAL_OUTPUT].data = 0;
	ssd1906_reg[REG_GPIO1_PULSE_START].data = 0;
	ssd1906_reg[REG_GPIO1_PULSE_STOP].data = 0;
	ssd1906_reg[REG_GPIO2_PULSE_DELAY].data = 0;
	ssd1906_reg[REG_STN_COLOR_DEPTH].data = 0;
	ssd1906_reg[REG_DYN_DITHER_CONTROL].data = 0;

	ssd1906_reg[REG_DISPLAY_MODE].data = 0;
	ssd1906_reg[REG_SPECIAL_EFFECTS].data = 0;
	ssd1906_reg[REG_MAIN_WIN_DISP_START_ADDR0].data = 0;
	ssd1906_reg[REG_MAIN_WIN_DISP_START_ADDR1].data = 0;
	ssd1906_reg[REG_MAIN_WIN_DISP_START_ADDR2].data = 0;
	ssd1906_reg[REG_MAIN_WIN_ADDR_OFFSET0].data = 0;
	ssd1906_reg[REG_MAIN_WIN_ADDR_OFFSET1].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_DISP_START_ADDR0].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_DISP_START_ADDR1].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_DISP_START_ADDR2].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_ADDR_OFFSET0].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_ADDR_OFFSET1].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_X_START_POS0].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_X_START_POS1].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_Y_START_POS0].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_Y_START_POS1].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_X_END_POS0].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_X_END_POS1].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_Y_END_POS0].data = 0;
	ssd1906_reg[REG_FLOAT_WIN_Y_END_POS1].data = 0;
	ssd1906_reg[REG_POWER_SAVE_CONFIG].data = 0;
	ssd1906_reg[REG_SOFTWARE_RESET].data = 0;
	ssd1906_reg[REG_SCRATCH_PAD0].data = 0;
	ssd1906_reg[REG_SCRATCH_PAD1].data = 0;
	ssd1906_reg[REG_GPIO_CONFIG0].data = 0;
	ssd1906_reg[REG_GPIO_CONFIG1].data = 0;
	ssd1906_reg[REG_GPIO_STATUS_CONTROL0].data = 0;
	ssd1906_reg[REG_GPIO_STATUS_CONTROL1].data = 0;
	ssd1906_reg[REG_PWM_CV_CLOCK_CONTROL].data = 0;
	ssd1906_reg[REG_PWM_CV_CLOCK_CONFIG].data = 0;
	ssd1906_reg[REG_CV_CLOCK_BURST_LENGTH].data = 0;
	ssd1906_reg[REG_PWM_CLOCK_DUTY_CYCLE].data = 0;

	ssd1906_reg[REG_CURSOR_FEATURE].data = 0;
	ssd1906_reg[REG_CURSOR1_BLINK_TOTAL0].data = 0;
	ssd1906_reg[REG_CURSOR1_BLINK_TOTAL1].data = 0;
	ssd1906_reg[REG_CURSOR1_BLINK_ON0].data = 0;
	ssd1906_reg[REG_CURSOR1_BLINK_ON1].data = 0;
	ssd1906_reg[REG_CURSOR1_MEM_START0].data = 0;
	ssd1906_reg[REG_CURSOR1_MEM_START1].data = 0;
	ssd1906_reg[REG_CURSOR1_MEM_START2].data = 0;
	ssd1906_reg[REG_CURSOR1_POSX0].data = 0;
	ssd1906_reg[REG_CURSOR1_POSX1].data = 0;
	ssd1906_reg[REG_CURSOR1_POSY0].data = 0;
	ssd1906_reg[REG_CURSOR1_POSY1].data = 0;
	ssd1906_reg[REG_CURSOR1_HORIZ_SIZE].data = 0;
	ssd1906_reg[REG_CURSOR1_VERT_SIZE].data = 0;
	ssd1906_reg[REG_CURSOR1_COL_IND1_0].data = 0;
	ssd1906_reg[REG_CURSOR1_COL_IND1_1].data = 0;
	ssd1906_reg[REG_CURSOR1_COL_IND2_0].data = 0;
	ssd1906_reg[REG_CURSOR1_COL_IND2_1].data = 0;
	ssd1906_reg[REG_CURSOR1_COL_IND3_0].data = 0;
	ssd1906_reg[REG_CURSOR1_COL_IND3_1].data = 0;
	ssd1906_reg[REG_CURSOR2_BLINK_TOTAL0].data = 0;
	ssd1906_reg[REG_CURSOR2_BLINK_TOTAL1].data = 0;
	ssd1906_reg[REG_CURSOR2_BLINK_ON0].data = 0;
	ssd1906_reg[REG_CURSOR2_BLINK_ON1].data = 0;
	ssd1906_reg[REG_CURSOR2_MEM_START0].data = 0;
	ssd1906_reg[REG_CURSOR2_MEM_START1].data = 0;
	ssd1906_reg[REG_CURSOR2_MEM_START2].data = 0;
	ssd1906_reg[REG_CURSOR2_POSX0].data = 0;
	ssd1906_reg[REG_CURSOR2_POSX1].data = 0;
	ssd1906_reg[REG_CURSOR2_POSY0].data = 0;
	ssd1906_reg[REG_CURSOR2_POSY1].data = 0;
	ssd1906_reg[REG_CURSOR2_HORIZ_SIZE].data = 0;
	ssd1906_reg[REG_CURSOR2_VERT_SIZE].data = 0;
	ssd1906_reg[REG_CURSOR2_COL_IND1_0].data = 0;
	ssd1906_reg[REG_CURSOR2_COL_IND1_1].data = 0;
	ssd1906_reg[REG_CURSOR2_COL_IND2_0].data = 0;
	ssd1906_reg[REG_CURSOR2_COL_IND2_1].data = 0;
	ssd1906_reg[REG_CURSOR2_COL_IND3_0].data = 0;
	ssd1906_reg[REG_CURSOR2_COL_IND3_1].data = 0;
	ssd1906_reg[REG_CIR].data = 0;
}
void _SSD1906_Delay (uint32_t time)
{
	uint32_t a;
	while(time--){
		a=25000;
		while (a--);
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _HW_Init_Flexbus_16b (void)
{
	SIM->SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;	//开启PORTA口使能时钟
	SIM->SCGC5|=SIM_SCGC5_PORTB_MASK;	//开启PORTB口使能时钟
	SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;	//开启PORTC口使能时钟
	SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;	//开启PORTD口使能时钟
	SIM->SCGC5|=SIM_SCGC5_PORTE_MASK;	//开启PORTE口使能时钟
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(0x4);
	//SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(0xf);
	FB->CS[0].CSAR = LCD_FLEX_BASE_ADDRESS;
	FB->CS[0].CSMR = FB_CSMR_V_MASK|LCD_FLEX_ADDRESS_MASK;
	FB->CS[0].CSCR = FB_CSCR_AA_MASK |FB_CSCR_PS(3) |FB_CSCR_BLS_MASK |FB_CSCR_WS(0x5)|FB_CSCR_RDAH(3)|FB_CSCR_WRAH(3);
	
	PORTD->PCR[GPIO_Pin_6] = PORT_PCR_MUX(5);/* FB_AD0 */
	PORTD->PCR[GPIO_Pin_5] = PORT_PCR_MUX(5);/* FB_AD1 */
	PORTD->PCR[GPIO_Pin_4] = PORT_PCR_MUX(5);/* FB_AD2 */
	PORTD->PCR[GPIO_Pin_3] = PORT_PCR_MUX(5);/* FB_AD3 */
	PORTD->PCR[GPIO_Pin_2] = PORT_PCR_MUX(5);/* FB_AD4 */
	PORTC->PCR[GPIO_Pin_10] = PORT_PCR_MUX(5);/*FB_AD5 */
	PORTC->PCR[GPIO_Pin_9] = PORT_PCR_MUX(5);/* FB_AD6 */
	PORTC->PCR[GPIO_Pin_8] = PORT_PCR_MUX(5);/* FB_AD7 */
	PORTC->PCR[GPIO_Pin_7] = PORT_PCR_MUX(5);/* FB_AD8 */
	PORTC->PCR[GPIO_Pin_6] = PORT_PCR_MUX(5);/* FB_AD9 */
	PORTC->PCR[GPIO_Pin_5] = PORT_PCR_MUX(5);/* FB_AD10 */
	PORTC->PCR[GPIO_Pin_4] = PORT_PCR_MUX(5);/* FB_AD11 */
	PORTC->PCR[GPIO_Pin_2] = PORT_PCR_MUX(5);/* FB_AD12 */
	PORTC->PCR[GPIO_Pin_1] = PORT_PCR_MUX(5);/* FB_AD13 */
	PORTC->PCR[GPIO_Pin_0] = PORT_PCR_MUX(5);/* FB_AD14 */
	PORTB->PCR[GPIO_Pin_18] = PORT_PCR_MUX(5);/* FB_AD15 */
	PORTB->PCR[GPIO_Pin_17] = PORT_PCR_MUX(5);/* FB_AD16 */
	PORTB->PCR[GPIO_Pin_16] = PORT_PCR_MUX(5);/* FB_AD17 */
	PORTB->PCR[GPIO_Pin_11] = PORT_PCR_MUX(5);/* FB_AD18 */
// 	PORTB->PCR[GPIO_Pin_10] = PORT_PCR_MUX(5);/* FB_AD19 */
// 	PORTB->PCR[GPIO_Pin_9] = PORT_PCR_MUX(5);/* FB_AD20 */
	
	PORTB->PCR[GPIO_Pin_19] = PORT_PCR_MUX(5);/* /FB_OE */
	PORTC->PCR[GPIO_Pin_11] = PORT_PCR_MUX(5);/* FB_RW */
	PORTD->PCR[GPIO_Pin_0] = PORT_PCR_MUX(5);/* FB_ALE */
	PORTD->PCR[GPIO_Pin_1] = PORT_PCR_MUX(5);/* FB_CS0 */
	
}

/*********************************END OF FILE**********************************/
