/*========================================================================
#   FileName: AT25DF.C
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-07 08:53:32
========================================================================*/
/* Includes ------------------------------------------------------------*/
#include "at25df.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
#define AT25_PORT_DATA (SPI1_SCK_PE2_SOUT_PE1_SIN_PE3)
#define AT25_PORT_CS (SPI1_PCS1_PB9)
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
static void _AT_delay10us (unsigned int count);//byte program time typ:7us
static void GPIO_Configuration (void);
static void SPI_Configuration (void);
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None
  * @retval None
  */
u8 b[256];
void AT_Configuration (void)
{
	u8 byte[4];
	u32 i=0;
	u32 addr = 0;
	SPI_Configuration ();
	AT_WriteEnable ();
	AT_ReadStatusRegister (byte);
	byte[0] &= 0xf3;
	AT_SetStatusRegByte1 (byte[0]);
}
/**
  * @brief  use this function,must be delay 32~56s.
  * @param  None
  * @retval None
  */
void AT_ChipErase (void)
{
	AT_WriteEnable ();
	AT_SendReceiveCont (0x60);
	AT_SendReceiveNcont (0xc7);
}
/**
  * @brief  use this function,must be delay 400~950ms.
  * @param  None
  * @retval None
  */
void AT_BlockErase64K (u32 addr)
{
	AT_WriteEnable ();
	AT_SendReceiveCont (0xd8);
	AT_SendReceiveCont (addr>>16);
	AT_SendReceiveCont (addr>>8);
	AT_SendReceiveNcont (addr);
}
/**
  * @brief  use this function,must be delay 250~600ms.
  * @param  None
  * @retval None
  */
void AT_BlockErase32K (u32 addr)
{
	AT_WriteEnable ();
	AT_SendReceiveCont (0x52);
	AT_SendReceiveCont (addr>>16);
	AT_SendReceiveCont (addr>>8);
	AT_SendReceiveNcont (addr);
}
/**
  * @brief  use this function,must be delay 50~200ms.
  * @param  None
  * @retval None
  */
void AT_BlockErase4K (u32 addr)
{
	AT_WriteEnable ();
	AT_SendReceiveCont (0x20);
	AT_SendReceiveCont (addr>>16);
	AT_SendReceiveCont (addr>>8);
	AT_SendReceiveNcont (addr);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void AT_WriteEnable (void)
{
	AT_SendReceiveNcont (0x06);
}
/**
  * @brief  use this function,must be delay 1~3ms.
  * @param  None
  * @retval None
  */
void AT_ProgramPage (u32 addr, u8 *data, u16 counter)
{	
	AT_WriteEnable ();
	AT_SendReceiveCont (0x02);
	AT_SendReceiveCont (addr>>16);
	AT_SendReceiveCont (addr>>8);
	AT_SendReceiveCont (addr);
	while (counter>1){
		AT_SendReceiveCont (*data);
		data++;
		counter --;
	}
	while (counter){
		AT_SendReceiveNcont (*data);
		counter --;
	}
}
/**
  * @brief  use this function,must be delay >7us.
  * @param  None
  * @retval None
  */
void AT_ProgramByte (u32 addr, u8 data)
{	
	AT_WriteEnable ();
	AT_SendReceiveCont (0x02);
	AT_SendReceiveCont (addr>>16);
	AT_SendReceiveCont (addr>>8);
	AT_SendReceiveCont (addr);
	AT_SendReceiveNcont (data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void AT_SetStatusRegByte1 (u8 data)
{
	AT_SendReceiveCont (0x01);
	AT_SendReceiveNcont (data);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void AT_ReadStatusRegister (u8 *data)
{
	AT_SendReceiveCont (0x05);
	data[0] = AT_SendReceiveCont (0);
	data[1] = AT_SendReceiveCont (0);
	data[2] = AT_SendReceiveNcont (0); 
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void AT_ReadMng_DrvID (u8 *data)
{
	AT_SendReceiveCont (0x9f);
	data[0] = AT_SendReceiveCont (0);
	data[1] = AT_SendReceiveCont (0);
	data[2] = AT_SendReceiveCont (0); 
	data[3] = AT_SendReceiveNcont (0);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void AT_ReadArrayByte (u32 addr, u8 *data)
{
	AT_SendReceiveCont (0x03);
	AT_SendReceiveCont (addr>>16);
	AT_SendReceiveCont (addr>>8);
	AT_SendReceiveCont (addr);
	*data = AT_SendReceiveNcont (0);
}
/**
  * @brief  over ,cs is high.
  * @param  None
  * @retval None
  */
u8 AT_SendReceiveNcont (u8 byte)
{
	u8 receive;
	while (AT25_SPI->SR & SPI_SR_TFFF_MASK == 0);
	AT25_SPI->PUSHR = (SPI_PUSHR_CTAS(1)
		//|SPI_PUSHR_CONT_MASK
		|SPI_PUSHR_PCS(1<<3)
		|SPI_PUSHR_TXDATA(byte));
	while (!(AT25_SPI->SR & SPI_SR_TCF_MASK));
	AT25_SPI->SR |= SPI_SR_TCF_MASK;
	while ((AT25_SPI->SR & SPI_SR_RFDF_MASK)==0);
	//printf("receive:%u\r\n",AT25_SPI->POPR);
	receive = AT25_SPI->POPR;
	while ((AT25_SPI->SR & SPI_SR_RFDF_MASK) == 0);
	return receive;
}
/**
  * @brief  keep cs low level.
  * @param  None
  * @retval None
  */
u8 AT_SendReceiveCont (u8 byte)
{
	u8 receive;
	while (AT25_SPI->SR & SPI_SR_TFFF_MASK == 0);
	AT25_SPI->PUSHR = (SPI_PUSHR_CTAS(1)
		|SPI_PUSHR_CONT_MASK
		|SPI_PUSHR_PCS(1<<3)
		|SPI_PUSHR_TXDATA(byte));
	while (!(AT25_SPI->SR & SPI_SR_TCF_MASK));
	AT25_SPI->SR |= SPI_SR_TCF_MASK;
	while ((AT25_SPI->SR & SPI_SR_RFDF_MASK)==0);
	//printf("receive:%u\r\n",AT25_SPI->POPR);
	receive = AT25_SPI->POPR;
	while ((AT25_SPI->SR & SPI_SR_RFDF_MASK) == 0);
	return receive;
}
static void _AT_delay10us (unsigned int count)
{
	u32 a;
	while (count--){
		a = 250;
		while (a--);
	}
}
static void GPIO_Configuration (void)
{

}
static void SPI_Configuration (void)
{
	/* å¼€å¯å¯¹åº”æ—¶ãÔ?*/
	SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	/* å¤á«”¨ */
//	AT25_SIN_PORT->PCR[AT25_SIN_Pin] &= ~PORT_PCR_MUX_MASK;
//	AT25_SOUT_PORT->PCR[AT25_SOUT_Pin] &= ~PORT_PCR_MUX_MASK;
//	AT25_SCK_PORT->PCR[AT25_SCK_Pin] &= ~PORT_PCR_MUX_MASK;
//	AT25_CS_PORT->PCR[AT25_CS_Pin] &= ~PORT_PCR_MUX_MASK;
//	AT25_SIN_PORT->PCR[AT25_SIN_Pin] |= PORT_PCR_MUX(2);
//	AT25_SOUT_PORT->PCR[AT25_SOUT_Pin] |= PORT_PCR_MUX(2);
//	AT25_SCK_PORT->PCR[AT25_SCK_Pin] |= PORT_PCR_MUX(2);
//	AT25_CS_PORT->PCR[AT25_CS_Pin] |= PORT_PCR_MUX(2);
	AT25_SIN_PORT->PCR[AT25_SIN_Pin] = (PORT_PCR_MUX(2) 
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	AT25_SOUT_PORT->PCR[AT25_SOUT_Pin] = (PORT_PCR_MUX(2)
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	AT25_SCK_PORT->PCR[AT25_SCK_Pin] = (PORT_PCR_MUX(2)
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	AT25_CS_PORT->PCR[AT25_CS_Pin] = (PORT_PCR_MUX(2)
			|PORT_PCR_PE_MASK
			|PORT_PCR_PS_MASK);
	/* ä¸»æ¨¡å¼?*/
	//AT25_SPI->MCR |= SPI_MCR_MSTR_MASK;	
	AT25_SPI->MCR  = 0 & (~SPI_MCR_MDIS_MASK) 
				|SPI_MCR_HALT_MASK
				|SPI_MCR_MSTR_MASK
				|SPI_MCR_PCSIS_MASK
				|SPI_MCR_CLR_TXF_MASK
				|SPI_MCR_CLR_RXF_MASK  
				|SPI_MCR_DIS_TXF_MASK
				|SPI_MCR_DIS_RXF_MASK
				|SPI_MCR_SMPL_PT(2);
	//é…á«½®åˆ†é¢‘åŠæ³¢ç‰¹çŽ‡
	AT25_SPI->CTAR[1] = 0| SPI_CTAR_DBR_MASK	 //è®¾ç½®ãÍÇâ¿¡ïÐ?
						| SPI_CTAR_PCSSCK(0)
						| SPI_CTAR_PASC(0)
						| SPI_CTAR_PBR(0)
						| SPI_CTAR_CSSCK(0)
						| SPI_CTAR_ASC (0)
					| SPI_CTAR_FMSZ(7)
					| SPI_CTAR_PDT(0);
	//åˆ†é¢‘è®¾ç½®
	AT25_SPI->CTAR[1] |=SPI_CTAR_BR(0x0);
	//ïß¶é’Ÿç›¸ä½ã€æž¸ä?
	//AT25_SPI->CTAR[1] |= SPI_CTAR_CPHA_MASK;
	//AT25_SPI->CTAR[1] |= SPI_CTAR_CPOL_MASK;
	//AT25_SPI->CTAR[1] |= SPI_CTAR_LSBFE_MASK;
	AT25_SPI->SR = SPI_SR_EOQF_MASK
		| SPI_SR_TFUF_MASK
		| SPI_SR_TFFF_MASK
		| SPI_SR_RFOF_MASK
		| SPI_SR_RFDF_MASK
		| SPI_SR_TCF_MASK;
	AT25_SPI->MCR &= ~SPI_MCR_HALT_MASK;
}
/*********************************END OF FILE****************************/ 

