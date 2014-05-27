/*========================================================================
#   FileName: ligsen.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-11-08 10:51:24
========================================================================*/
/* Includes ------------------------------------------------------------*/
#include "..\..\bsp\fwlib\gpio.h"
#include "..\..\bsp\fwlib\adc.h"
#include "ligsen.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
static void ADC_Configuration (void);
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void LIG_Configuration (void)
{
//	unsigned short value;
	ADC_Configuration ();
//	while (1){
//		value = ADC_GetConversionValue (ADC0_SE13_PB3);
//		printf ("ad value:%x\r\n",value);
//		STK_delay10ms (50);
//	}
}

static void ADC_Configuration (void)
{
	unsigned short cal_var;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	LIG_PORT->PCR[LIG_PORT_Pin] &= ~PORT_PCR_MUX_MASK;
	LIG_PORT->PCR[LIG_PORT_Pin] |= PORT_PCR_MUX(0);
	LIG_ADC->CFG1 &= ~(ADC_CFG1_MODE_MASK);
	LIG_ADC->CFG1 |= ADC_CFG1_MODE (3);					//
	LIG_ADC->CFG1 &= ~(ADC_CFG1_ADICLK_MASK);
	LIG_ADC->CFG1 |= (ADC_CFG1_ADICLK (0));				//
	LIG_ADC->CFG1 &= ~(ADC_CFG1_ADLSMP_MASK);			//
	LIG_ADC->CFG1 &= ~(ADC_CFG1_ADIV_MASK);
	LIG_ADC->CFG1 |= ADC_CFG1_ADIV (3);					//¢‘
	LIG_ADC->CFG2 = 0;									//
	LIG_ADC->CFG2 |= (ADACKEN_DISABLED|ADHSC_HISPEED
			|ADC_CFG2_ADLSTS(ADLSTS_20));
	LIG_ADC->CV1 = 0x1234u; 
	LIG_ADC->CV2 = 0x5678u;
	LIG_ADC->SC2 = (ACFE_DISABLED|ACFGT_GREATER
			|ACREN_ENABLED|DMAEN_DISABLED
			|ADC_SC2_REFSEL(REFSEL_EXT));
	LIG_ADC->SC3 = (CAL_OFF|ADCO_SINGLE|AVGE_ENABLED
			|ADC_SC3_AVGS(AVGS_32));
	LIG_ADC->PGA = (PGAEN_DISABLED|PGACHP_NOCHOP|PGALP_NORMAL
			|ADC_PGA_PGAG(PGAG_64));
	LIG_ADC->SC1[0] &= ~ADC_SC1_DIFF_MASK;//
	
	LIG_ADC->SC2 &= ~ADC_SC2_ADTRG_MASK;
	LIG_ADC->SC3 &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); 
	LIG_ADC->SC3 |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(AVGS_32) );
	LIG_ADC->SC3 |= ADC_SC3_CAL_MASK ;
	while((LIG_ADC->SC1[0] & ADC_SC1_COCO_MASK) == 0);
	if((LIG_ADC->SC3 & ADC_SC3_CALF_MASK) == CALF_FAIL){		
	}else{
		cal_var = 0x00;
		cal_var =  LIG_ADC->CLP0; 
		cal_var += LIG_ADC->CLP1; 
		cal_var += LIG_ADC->CLP2; 
		cal_var += LIG_ADC->CLP3; 
		cal_var += LIG_ADC->CLP4; 
		cal_var += LIG_ADC->CLPS; 
		cal_var = cal_var/2;
		cal_var |= 0x8000; // Set MSB
		LIG_ADC->PG =  ADC_PG_PG(cal_var);
		// Calculate minus-side calibration
		cal_var = 0x00;
		cal_var =  LIG_ADC->CLM0; 
		cal_var += LIG_ADC->CLM1; 
		cal_var += LIG_ADC->CLM2; 
		cal_var += LIG_ADC->CLM3; 
		cal_var += LIG_ADC->CLM4; 
		cal_var += LIG_ADC->CLMS; 
		cal_var = cal_var/2;
		cal_var |= 0x8000; // Set MSB
		LIG_ADC->MG = ADC_MG_MG(cal_var); 
		LIG_ADC->SC3 &= ~ADC_SC3_CAL_MASK;
	}
}
/*********************************END OF FILE****************************/


