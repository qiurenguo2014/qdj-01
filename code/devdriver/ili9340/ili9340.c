/*========================================================================
#   FileName: ili9340.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-12 13:54:32
========================================================================*/
/*
 *	3-Wire 9-Bit Serial Interface
 *	MSB-----------------------------------------LSB
 *	D/C--D7-------------------------------------D0
 *	Write Mode
 *	_____^__^__^__^__^__^__^__^__^______________scl
 *	D/C 0:TRAN COMMAND 1:TRAN DATA
 * */
/* Includes ------------------------------------------------------------*/
#include <stdint.h>
#include "../../bsp/fwlib/gpio.h"
#include "ili9340.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/

/* Variables -----------------------------------------------------------*/

/* Functions prototypes ------------------------------------------------*/
static void SPI_Configuration (void);
static void GPIO_Configuration (void);
static uint16_t ILI_SendReceiveCont (uint16_t data);
static uint16_t ILI_SendReceiveNcont (uint16_t data);
static void _ILI_Delay1ms (uint32_t delay);
static void _ILI_SendDummyData (void);
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void ILI_Configuration (void)
{
	GPIO_Configuration ();
	SPI_Configuration ();
	ILI_Initial ();
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void ILI_Initial (void)
{
	ILI_ResetDevice ();
	/* sleep out */
	ILI_SendReceiveNcont (0x1ff);
	ILI_WriteCmd (0x11);	 //sleep out
	_ILI_Delay1ms (100);

	ILI_WriteCmd(0xF7);
	ILI_WriteData(0x20);
	ILI_WriteCmd(0xEA);
	ILI_WriteData(0x00);
	ILI_WriteData(0x00);
#if 1
	ILI_WriteCmd(0xCF);
	ILI_WriteData(0x00);
	ILI_WriteData(0x81);
	ILI_WriteData(0X30);
	ILI_WriteCmd(0xED);
	ILI_WriteData(0x64);
	ILI_WriteData(0x03);
	ILI_WriteData(0X12);
	ILI_WriteData(0X81);
	ILI_WriteCmd(0xE8);
	ILI_WriteData(0x85);
	ILI_WriteData(0x10);
	ILI_WriteData(0x7A);
#endif
	ILI_WriteCmd(0xcb); //power control
	ILI_WriteData(0x39);
	ILI_WriteData(0x2C);
	ILI_WriteData(0x00);
	ILI_WriteData(0x34);
	ILI_WriteData(0x02);

	ILI_WriteCmd(0xC0); //power control
	ILI_WriteData(0x38);

	ILI_WriteCmd(0xc1); //power control
	ILI_WriteData(0x10);

	ILI_WriteCmd(0xc5); //vcom control
	ILI_WriteData(0x35);
	ILI_WriteData(0x10);
	ILI_WriteCmd(0x36); // memory access control
	ILI_WriteData(0x20); // my,mx,mv,ml,BGR,mh,0.0

	ILI_WriteCmd(0xb1); //frame rate
	ILI_WriteData(0x00);
	ILI_WriteData(0x16); //70

	ILI_WriteCmd(0xb6); // display function control
	ILI_WriteData(0x0a);
	ILI_WriteData(0x82);

	ILI_WriteCmd(0xc7); //vcom control
	ILI_WriteData(0xC4);

	ILI_WriteCmd(0xf2); // 3Gamma Function Disable
	ILI_WriteData(0x08);

	ILI_WriteCmd(0x26);
	ILI_WriteData(0x01); // gamma set 4 gamma curve 01/02/04/08
	ILI_WriteCmd(0x3a); // pixel format set
	ILI_WriteData(0x66);	 //18bit /pixel RGB
	ILI_WriteCmd(0xB0); // pixel format set
	ILI_WriteData(0x40);	 //16bit /pixel

	ILI_WriteCmd(0xF6); // pixel format set
	ILI_WriteData(0x81);	 //16bit /pixel
	ILI_WriteData(0x00);	 //16bit /pixel
	ILI_WriteData(0x06);	 //16bit /pixel

	ILI_WriteCmd(0xE0); //positive gamma correction
	ILI_WriteData(0x1f);
	ILI_WriteData(0x1a);
	ILI_WriteData(0x18);
	ILI_WriteData(0x0a);
	ILI_WriteData(0x0f);
	ILI_WriteData(0x06);
	ILI_WriteData(0x45);
	ILI_WriteData(0x87);
	ILI_WriteData(0x32);
	ILI_WriteData(0x0a);
	ILI_WriteData(0x07);
	ILI_WriteData(0x02);
	ILI_WriteData(0x07);
	ILI_WriteData(0x05);
	ILI_WriteData(0x00);

	ILI_WriteCmd(0xE1); //ne gamma correction
	ILI_WriteData(0x00);
	ILI_WriteData(0x25);
	ILI_WriteData(0x27);
	ILI_WriteData(0x05);
	ILI_WriteData(0x10);
	ILI_WriteData(0x09);
	ILI_WriteData(0x3a);
	ILI_WriteData(0x78);
	ILI_WriteData(0x4d);
	ILI_WriteData(0x05);
	ILI_WriteData(0x18);
	ILI_WriteData(0x0d);
	ILI_WriteData(0x38);
	ILI_WriteData(0x3a);
	ILI_WriteData(0x1f);
	ILI_WriteCmd(0x29); // display on
	ILI_WriteCmd(0x35);
	ILI_WriteData(0x01);
	ILI_WriteCmd(0x38);//ILI_WriteCmd(0x39);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void ILI_Read_3Bytes (uint16_t cmd,uint32_t *data)
{
	ILI_SendReceiveCont (cmd);
	data[0] = ILI_SendReceiveCont (0x1ff);
	data[0] <<= 9;
	data[0] |= ILI_SendReceiveCont (0x1ff);
	data[0] <<= 9;
	data[0] |= ILI_SendReceiveNcont (0x1ff);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void ILI_Read_1Byte (uint16_t cmd, uint32_t *data)
{
	ILI_SendReceiveCont (cmd);
	data[0] = ILI_SendReceiveNcont (0x1ff);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void ILI_WriteData (uint16_t data)
{
	ILI_SendReceiveNcont (data|0x100);
	_ILI_SendDummyData ();
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void ILI_WriteCmd (uint16_t cmd)
{
	ILI_SendReceiveNcont (cmd);
	_ILI_SendDummyData ();
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void _ILI_SendDummyData (void)
{
	uint16_t receive;
	while (ILI_SPI->SR & SPI_SR_TFFF_MASK == 0);
	ILI_SPI->PUSHR = (SPI_PUSHR_CTAS(0)
			|SPI_PUSHR_TXDATA(0x1ff));
	while (!(ILI_SPI->SR&SPI_SR_TCF_MASK));
	ILI_SPI->SR |= SPI_SR_TCF_MASK;
	while ((ILI_SPI->SR & SPI_SR_RFDF_MASK)==0);
	receive = ILI_SPI->POPR;
	while ((ILI_SPI->SR & SPI_SR_RFDF_MASK) == 0);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint16_t ILI_SendReceiveNcont (uint16_t data)
{
	uint16_t receive;
	while (ILI_SPI->SR & SPI_SR_TFFF_MASK == 0);
	ILI_SPI->PUSHR = (SPI_PUSHR_CTAS(0)
			|SPI_PUSHR_PCS(1<<1)
			|SPI_PUSHR_TXDATA(data));
	while (!(ILI_SPI->SR&SPI_SR_TCF_MASK));
	ILI_SPI->SR |= SPI_SR_TCF_MASK;
	while ((ILI_SPI->SR & SPI_SR_RFDF_MASK)==0);
	receive = ILI_SPI->POPR;
	while ((ILI_SPI->SR & SPI_SR_RFDF_MASK) == 0);
	return receive;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint16_t ILI_SendReceiveCont (uint16_t data)
{
	uint16_t receive;
	while (ILI_SPI->SR & SPI_SR_TFFF_MASK == 0);
	ILI_SPI->PUSHR = (SPI_PUSHR_CTAS(0)
			|SPI_PUSHR_CONT_MASK
			|SPI_PUSHR_PCS(1<<1)
			|SPI_PUSHR_TXDATA(data));
	while (!(ILI_SPI->SR&SPI_SR_TCF_MASK));
	ILI_SPI->SR |= SPI_SR_TCF_MASK;
	while ((ILI_SPI->SR & SPI_SR_RFDF_MASK)==0);
	receive = ILI_SPI->POPR;
	while ((ILI_SPI->SR & SPI_SR_RFDF_MASK) == 0);
	return receive;
}
/**
  * @brief  ssd1906!!.
  * @param  None
  * @retval None
  */
void ILI_ResetDevice (void)
{
	uint32_t delay=10000;
	ILI_RESET_BITSET
	while (delay--);
	delay = 10000;
	ILI_RESET_BITRESET
	while (delay--);
	ILI_RESET_BITSET
}
void _ILI_Delay1ms (uint32_t delay)
{
	uint32_t MS;
	while (delay--){
		MS = 25000;
		while (MS--);
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SPI_Configuration (void)
{
	/* note:ILI初始化需在AT25芯片初始化之�?*/
	/* 开启对应时��?*/
	SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	ILI_SIN_PORT->PCR[ILI_SIN_Pin] = (PORT_PCR_MUX(2) 
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	ILI_SOUT_PORT->PCR[ILI_SOUT_Pin] = (PORT_PCR_MUX(2)
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	ILI_SCK_PORT->PCR[ILI_SCK_Pin] = (PORT_PCR_MUX(2)
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	ILI_CS_PORT->PCR[ILI_CS_Pin] = (PORT_PCR_MUX(2)
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	/* 主模�?*/
	ILI_SPI->MCR  = 0 & (~SPI_MCR_MDIS_MASK) 
				|SPI_MCR_HALT_MASK
				|SPI_MCR_MSTR_MASK
				|SPI_MCR_PCSIS_MASK
				|SPI_MCR_CLR_TXF_MASK
				|SPI_MCR_CLR_RXF_MASK  
				|SPI_MCR_DIS_TXF_MASK
				|SPI_MCR_DIS_RXF_MASK
				|SPI_MCR_SMPL_PT(2);
	//�᫽�分频及波特率
	ILI_SPI->CTAR[0] = 0| SPI_CTAR_DBR_MASK	 //设置���⿡��?
						| SPI_CTAR_PCSSCK(0)
						| SPI_CTAR_PASC(0)
						| SPI_CTAR_PBR(0)
						| SPI_CTAR_CSSCK(0)
						| SPI_CTAR_ASC (0)
					| SPI_CTAR_FMSZ(8)
					| SPI_CTAR_PDT(0);
	//分频设置
	ILI_SPI->CTAR[0] |=SPI_CTAR_BR(0xc);
	//�߶钟相位、极��?
	//ILI_SPI->CTAR[0] |= SPI_CTAR_CPHA_MASK;
	//ILI_SPI->CTAR[0] |= SPI_CTAR_CPOL_MASK;
	//ILI_SPI->CTAR[0] |= SPI_CTAR_LSBFE_MASK;
	ILI_SPI->SR = SPI_SR_EOQF_MASK
		| SPI_SR_TFUF_MASK
		| SPI_SR_TFFF_MASK
		| SPI_SR_RFOF_MASK
		| SPI_SR_RFDF_MASK
		| SPI_SR_TCF_MASK;
	ILI_SPI->MCR &= ~SPI_MCR_HALT_MASK;
}
void GPIO_Configuration (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIOx = ILI_RESET;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = ILI_RESET_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_Init (&GPIO_InitStruct);
}
/*********************************END OF FILE****************************/


