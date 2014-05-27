/*========================================================================
#   FileName: ad7687.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-29 16:39:33
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include "ad7687.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
static void _AD7687_delay (void);
static void _AD7687_Send_aclock (void);
static void GPIO_Configuration (void);
static void SPI_Configuration (void);
static void FTM_Configuration (void);
static void PIT_Configuration (void);
void AD7687_SetConverPin (void);
/* Functions --------------------------------------------------------*/
#define SPILCD_PORT_DATA     (SPI0_SCK_PA15_SOUT_PA16_SIN_PA17)
#define SPILCD_PORT_CS       (SPI0_PCS0_PA14)
void AD7687_Configuration (void)
{
	SPI_Configuration ();
// 	GPIO_Configuration ();
// 	while(1){
// 		AD7687_ResetConverPin ();
// 		STK_delay10ms (1);
// 		AD7687_SetConverPin ();
// 		STK_delay10ms (200);
// 	}
	//PIT_Configuration ();
	FTM_Configuration ();
}
/**
  * @brief  绛夊緟杞崲缁撴潫銆?50kps 锛?寰
  *		Table 9. Output Codes and Ideal Input Voltages 
  *		Description			Analog Input     Digital Output Code Hexa 
  *							 VREF= 5 V
  *		FSR 钬?1LSB			+4.999847V			7FFF
  *		Midscale + 1 LSB  	+152.6 渭V  			0001 
  *		Midscale  			0 V  				0000 
  *		Midscale 钬?1 LSB  	鈭?52.6 渭V  			FFFF 
  *		钬揊SR + 1 LSB  		鈭?.999847 V  		8001 
  *		钬揊SR  				鈭? V  				8000
  * @param  None
  * @retval 0锛氩け璐ワ紝瓒呮椂 1锛氭垚锷?
  */
uint32_t AD7687_WaitConverOver (void)
{
	uint32_t ret = 0;
 	while (GPIO_ReadInputDataBit (AD7687_SIN, AD7687_SIN_Pin)==1){
 	}
	while (GPIO_ReadInputDataBit (AD7687_SIN, AD7687_SIN_Pin)==0){
	}
	return ret;
}
void AD7687_ResetConverPin (void)
{
	GPIO_ResetBits (AD7687_CNV, AD7687_CNV_Pin);
}
void AD7687_SetConverPin (void)
{
	GPIO_SetBits (AD7687_CNV, AD7687_CNV_Pin);
}
/**
  * @brief  get two ad7687 conver data,
  * @param  None
  * @retval None
  */
void AD7687_GetConverData (uint16_t *ad)
{
	_AD7687_Send_aclock ();	
	while((AD7687_SPI->SR & SPI_SR_TFFF_MASK) == 0){};		
	AD7687_SPI->PUSHR = SPI_PUSHR_CTAS(1)
						| SPI_PUSHR_TXDATA(0);		
	while(!(AD7687_SPI->SR & SPI_SR_TCF_MASK)){};		
	AD7687_SPI->SR |= SPI_SR_TCF_MASK;		
	while((AD7687_SPI->SR & SPI_SR_RFDF_MASK) == 0);   //RX FIFO 未接收到数据则一直等待
	ad[0] = AD7687_SPI->POPR;							//B通道 交流电流 正极电压
	while((AD7687_SPI->SR & SPI_SR_RFDF_MASK) == 0){};   //RX FIFO 未接收到数据则一直等待

	while((AD7687_SPI->SR & SPI_SR_TFFF_MASK) == 0){};
	AD7687_SPI->PUSHR = SPI_PUSHR_CTAS(1)
						| SPI_PUSHR_TXDATA(0);
	while(!(AD7687_SPI->SR & SPI_SR_TCF_MASK)){};
	AD7687_SPI->SR |= SPI_SR_TCF_MASK;
	while((AD7687_SPI->SR & SPI_SR_RFDF_MASK) == 0){};
	ad[1] = AD7687_SPI->POPR;							////A通道 交流电压 负极电压
	while((AD7687_SPI->SR & SPI_SR_RFDF_MASK) == 0){};   //RX FIFO 未接收到数据则一直等待
}
/**
  * @brief  When SDI and CNV are low, SDO is driven low. With SCK 
  *			high, a rising edge on CNV initiates a conversion.
  *			寮�鍚疨WM锛屽苟镓揿紑SIN镄勪笂鍗囨部涓柇
  *
  * @param  None
  * @retval None
  */
void AD7687_StartConver (void)
{
	FTM1->CNT = 0;
	FTM_PWM_ChangeDuty(FTM1_CH1_PB1,FTM_DUTY);
	NVIC_EnableIRQ (AD7687_SIN_IRQ);
}
/**
  * @brief  鍏抽棴sin涓柇锛屽叧闂璓WM 
  * @param  None
  * @retval None
  */
void AD7687_StopConver (void)
{
	NVIC_DisableIRQ (AD7687_SIN_IRQ);
	FTM_PWM_ChangeDuty(FTM1_CH1_PB1,0);
}
void AD7687_SendWord (uint16_t data)
{
	SPI_ReadWriteByte(SPI0_PCS0_PA14,data,SPI_PCS_Inactive);
}
uint16_t AD7687_ReceiveWord(void)
{
    uint16_t data;
    data = SPI_ReadWriteByte(SPI0_PCS0_PA14,0,SPI_PCS_Inactive);
    return data;
}
void _AD7687_delay (void)
{
	uint32_t delay = 10;
	while (delay--);
}
void _AD7687_Send_aclock (void)
{
	AD7687_SCK_PORT->PCR[AD7687_SCK_Pin]&=~(PORT_PCR_MUX_MASK);
	AD7687_SCK_PORT->PCR[AD7687_SCK_Pin]|=PORT_PCR_MUX(1);
	AD7687_SCK->PDDR |= (1<<AD7687_SCK_Pin);
	AD7687_SCK_PORT->PCR[AD7687_SCK_Pin] |= PORT_PCR_ODE_MASK;
	GPIO_ResetBits (AD7687_SCK, AD7687_SCK_Pin);
	_AD7687_delay ();
	GPIO_SetBits (AD7687_SCK, AD7687_SCK_Pin);
	AD7687_SCK_PORT->PCR[AD7687_SCK_Pin]&=~(PORT_PCR_MUX_MASK);    
	AD7687_SCK_PORT->PCR[AD7687_SCK_Pin]|=PORT_PCR_MUX(2);
}
void SPI_Configuration (void)
{
	SPI_Type *SPIx = NULL;
	PORT_Type *SPI_PORT = NULL;
	SPI_InitTypeDef SPI_InitStruct;
	SPI_DataMapTypeDef *pSPI_DataMap;
	SPI_CSMapTypeDef *pSPI_CSMap;
	SPI_InitStruct.SPIxDataMap = SPILCD_PORT_DATA;
	SPI_InitStruct.SPIxPCSMap = SPILCD_PORT_CS;
	SPI_InitStruct.SPI_DataSize = 16;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_Init(&SPI_InitStruct);
	pSPI_CSMap = (SPI_CSMapTypeDef*)&(SPI_InitStruct.SPIxPCSMap);
	pSPI_DataMap = (SPI_DataMapTypeDef*)&(SPI_InitStruct.SPIxDataMap);
	SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;
	SPIx = SPI0;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SPI_PORT = PORTA;	
	SPI_PORT->PCR[pSPI_DataMap->SPI_SCK_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	SPI_PORT->PCR[pSPI_DataMap->SPI_SIN_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	SPI_PORT->PCR[pSPI_DataMap->SPI_SOUT_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	SPI_PORT->PCR[pSPI_DataMap->SPI_SCK_Pin_Index] |= PORT_PCR_MUX(pSPI_DataMap->SPI_Alt_Index);
	SPI_PORT->PCR[pSPI_DataMap->SPI_SIN_Pin_Index] |= PORT_PCR_MUX(pSPI_DataMap->SPI_Alt_Index);
	SPI_PORT->PCR[pSPI_DataMap->SPI_SOUT_Pin_Index] |= PORT_PCR_MUX(pSPI_DataMap->SPI_Alt_Index);
	/*SCK配置开漏*/
	SPI_PORT->PCR[pSPI_DataMap->SPI_SCK_Pin_Index]|= PORT_PCR_ODE_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
/*	SPI_PORT = PORTA;
	SPI_PORT->PCR[pSPI_CSMap->SPI_PCS_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	SPI_PORT->PCR[pSPI_CSMap->SPI_PCS_Pin_Index] |= PORT_PCR_MUX(pSPI_CSMap->SPI_Alt_Index);*/
	//设置主从模式
	(SPI_InitStruct.SPI_Mode == SPI_Mode_Master)?(SPIx->MCR  |= SPI_MCR_MSTR_MASK):(SPIx->MCR  &= ~SPI_MCR_MSTR_MASK);
	SPIx->MCR  = 0 & (~SPI_MCR_MDIS_MASK) 
					|SPI_MCR_HALT_MASK        //让SPI进入停止模式
					|SPI_MCR_MSTR_MASK        //配置SPI为主机模式
					|SPI_MCR_PCSIS_MASK       //PCS为高电平当在SPI不工作的时候
					|SPI_MCR_CLR_TXF_MASK     //首先要清除MDIS，清除TXF_MASK和RXF_MASK
					|SPI_MCR_CLR_RXF_MASK  
					|SPI_MCR_DIS_TXF_MASK     //然后再禁止TXD和RXD FIFO 模式 ，将SPI配置成正常模式
					|SPI_MCR_DIS_RXF_MASK
					|SPI_MCR_SMPL_PT(2);
	SPIx->CTAR[1] = 0| SPI_CTAR_DBR_MASK	 //设置通信的
					| SPI_CTAR_PCSSCK(0)
					| SPI_CTAR_PASC(0)
					| SPI_CTAR_ASC(0)
					| SPI_CTAR_PBR(0)
					| SPI_CTAR_CSSCK(0)
					| SPI_CTAR_FMSZ(SPI_InitStruct.SPI_DataSize -1)//设置数据传输的位数
					| SPI_CTAR_PDT(0);//设置片选信号在数据完成后的延时值 
	//分频设置
	SPIx->CTAR[1] |=SPI_CTAR_BR(SPI_InitStruct.SPI_BaudRatePrescaler);							 
	//时钟相位设置
	(SPI_InitStruct.SPI_CPHA == SPI_CPHA_1Edge)?(SPIx->CTAR[1] &= ~SPI_CTAR_CPHA_MASK):(SPIx->CTAR[1] |= SPI_CTAR_CPHA_MASK);
	//时钟极性
	(SPI_InitStruct.SPI_CPOL == SPI_CPOL_Low)?(SPIx->CTAR[1] &= ~SPI_CTAR_CPOL_MASK):(SPIx->CTAR[1] |= SPI_CTAR_CPOL_MASK);
	//配置MSB或者LSD
	(SPI_InitStruct.SPI_FirstBit == SPI_FirstBit_MSB)?(SPIx->CTAR[1] &= ~SPI_CTAR_LSBFE_MASK):(SPIx->CTAR[1] |= SPI_CTAR_LSBFE_MASK);
	//清空状态
	SPIx->SR = SPI_SR_EOQF_MASK   //队列结束标志 w1c  (write 1 to clear)     
			| SPI_SR_TFUF_MASK    //TX FIFO underflow flag  w1c
			| SPI_SR_TFFF_MASK    //TX FIFO fill      flag  w1c
			| SPI_SR_RFOF_MASK    //RX FIFO overflow  flag  w1c
			| SPI_SR_RFDF_MASK    //RX FIFO fill      flasg w1c (0时为空)
			| SPI_SR_TCF_MASK;
	//开始传输
	SPIx->MCR &= ~SPI_MCR_HALT_MASK;    //开始传输，见参考手册1129页					
	
	AD7687_ReceiveWord ();
	NVIC_DisableIRQ (AD7687_SIN_IRQ);
 	AD7687_SIN_PORT->PCR[AD7687_SIN_Pin]&=~PORT_PCR_IRQC_MASK;
 	AD7687_SIN_PORT->PCR[AD7687_SIN_Pin]|=PORT_PCR_IRQC(GPIO_IT_RISING);
}
void PIT_Configuration (void)
{
	PIT_InitTypeDef PIT_InitStruct1;
	PIT_InitStruct1.PITx = PIT0;          //PIT0通道
	PIT_InitStruct1.PIT_Interval = 200;   //定时周期100MS
	PIT_Init(&PIT_InitStruct1);
	PIT_ITConfig(PIT0,PIT_IT_TIF,ENABLE);
	//NVIC_EnableIRQ(PIT0_IRQn);
}
void FTM_Configuration (void)
{
	FTM_InitTypeDef FTM_InitStruct;
	FTM_PWM_MapTypeDef* pFTM_Map = (FTM_PWM_MapTypeDef*) FTM1;
	FTM_InitStruct.Frequency = (uint32_t)(4096*5);  //
	FTM_InitStruct.FTMxMAP = FTM1_CH1_PB1;          //
	FTM_InitStruct.FTM_Mode = FTM_Mode_EdgeAligned; //
	FTM_InitStruct.InitalDuty = FTM_DUTY;           //0~10000
	FTM_Init(&FTM_InitStruct);
	AD7687_CNV_PORT->PCR[AD7687_CNV_Pin]|= PORT_PCR_ODE_MASK;
	FTM_PWM_ChangeDuty(FTM1_CH1_PB1,0);
}
void GPIO_Configuration (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIOx = AD7687_CNV;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Pin = AD7687_CNV_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OOD;
	GPIO_Init (&GPIO_InitStruct);
}
/*********************************END OF FILE**********************************/
/*
	while (1){
		AD7687_StartConver ();
		if (AD7687_judgeConverOver()){
			data1 = AD7687_GetConverData();	
			AD7687_ResetConverPin ();
			printf ("receive1:%x\r\n",data1&0xffff);
			printf ("receive2:%x\r\n",data1>>16);
		}
		STK_delay10ms (200);
	}
*/


