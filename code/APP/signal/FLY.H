/*************************************************************************
#    FileName: FLY.H
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-01-15 15:38:33
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __FLY_H
#define __FLY_H
#include "sig_process.h"
/* Includes ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------*/
/* Exported constants --------------------------------------------------*/
/* Exported define -----------------------------------------------------*/
/* Exported variables --------------------------------------------------*/
extern const float cos_tab[];
extern const float sin_tab[];
#define PI 3.1415926535897932384626433832795028841971        
#define FFT_N DEFAULT_PERIOD
typedef struct{
	float real;
	float imag;
}compx;

/* Exported functions prototypes ---------------------------------------*/
extern void FFT(compx *xin);
extern void FFT1(float xreal [], float ximag [], int n);
/*  */
extern void THD(float xreal [], float ximag [], float *thd);
#endif 
/*********************************END OF FILE****************************/


