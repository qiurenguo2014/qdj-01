/*************************************************************************
#    FileName: fft.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-02-11 14:32:15
*************************************************************************/
#ifndef _FFT_H
#define _FFT_H
extern void kkfft(double pr[], double pi[], int n, int k, double fr[], double fi[], int l, int il);
extern void fft(float pr[], float pi[], int n, int k, float fr[], float fi[]);
extern void fft_CalcHarmonic (int h, const float *fft, const int n,float *real, float *imag);
#endif
