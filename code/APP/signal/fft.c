#include <math.h>
#include "fft.h"
// 函数名: 快速傅立叶变换（来源《C常用算法集》）
// 本函数测试OK,可以在TC2.0,VC++6.0,Keil C51测试通过。
// 如果你的MCS51系统有足够的RAM时,可以验证一下用单片机处理FFT有多么的慢。
//
// 入口参数： 
// l: l = 0, 傅立叶变换; l = 1, 逆傅立叶变换
// il: il = 0,不计算傅立叶变换或逆变换模和幅角；il = 1,计算模和幅角
// n: 输入的点数，为偶数，一般为32，64，128，...,1024等
// k: 满足n=2^k(k>0),实质上k是n个采样数据可以分解为偶次幂和奇次幂的次数
// pr[]: l=0时，存放N点采样数据的实部
// l=1时, 存放傅立叶变换的N个实部
// pi[]: l=0时，存放N点采样数据的虚部 
// l=1时, 存放傅立叶变换的N个虚部
//
// 出口参数：
// fr[]: l=0, 返回傅立叶变换的实部
// l=1, 返回逆傅立叶变换的实部
// fi[]: l=0, 返回傅立叶变换的虚部
// l=1, 返回逆傅立叶变换的虚部
// pr[]: il = 1,i = 0 时，返回傅立叶变换的模
// il = 1,i = 1 时，返回逆傅立叶变换的模
// pi[]: il = 1,i = 0 时，返回傅立叶变换的辐角
// il = 1,i = 1 时，返回逆傅立叶变换的辐角
// data: 2005.8.15,Mend Xin Dong
void kkfft(double pr[], double pi[], int n, int k, double fr[], double fi[], int l, int il)
{
int it,m,is,i,j,nv,l0;
double p,q,s,vr,vi,poddr,poddi;

for (it=0; it<=n-1; it++)
{ 
   m = it; 
   is = 0;
   for(i=0; i<=k-1; i++)
   { 
    j = m/2; 
    is = 2*is+(m-2*j); 
    m = j;
   }
   fr[it] = pr[is];
   fi[it] = pi[is];
}
//----------------------------
pr[0] = 1.0; 
pi[0] = 0.0;
p = 6.283185306/(1.0*n);
pr[1] = cos(p); 
pi[1] = -sin(p);

if (l!=0) 
   pi[1]=-pi[1];

for (i=2; i<=n-1; i++)
{ 
   p = pr[i-1]*pr[1]; 
   q = pi[i-1]*pi[1];
   s = (pr[i-1]+pi[i-1])*(pr[1]+pi[1]);
   pr[i] = p-q; 
   pi[i] = s-p-q;
}

for (it=0; it<=n-2; it=it+2)
{ 
   vr = fr[it]; 
   vi = fi[it];
   fr[it] = vr+fr[it+1]; 
   fi[it] = vi+fi[it+1];
   fr[it+1] = vr-fr[it+1]; 
   fi[it+1] = vi-fi[it+1];
}
m = n/2; 
nv = 2;

for (l0=k-2; l0>=0; l0--)
{ 
   m = m/2; 
   nv = 2*nv;
   for(it=0; it<=(m-1)*nv; it=it+nv)
    for (j=0; j<=(nv/2)-1; j++)
    { 
     p = pr[m*j]*fr[it+j+nv/2];
     q = pi[m*j]*fi[it+j+nv/2];
     s = pr[m*j]+pi[m*j];
     s = s*(fr[it+j+nv/2]+fi[it+j+nv/2]);
     poddr = p-q; 
     poddi = s-p-q;
     fr[it+j+nv/2] = fr[it+j]-poddr;
     fi[it+j+nv/2] = fi[it+j]-poddi;
     fr[it+j] = fr[it+j]+poddr;
     fi[it+j] = fi[it+j]+poddi;
    }
}

if(l!=0)
{
   for(i=0; i<=n-1; i++)
   { 
    fr[i] = fr[i]/(1.0*n);
    fi[i] = fi[i]/(1.0*n);
   }
} 
if(il!=0)
{
   for(i=0; i<=n-1; i++)
   { 
    pr[i] = sqrt(fr[i]*fr[i]+fi[i]*fi[i]);
    if(fabs(fr[i])<0.000001*fabs(fi[i]))
    { 
     if ((fi[i]*fr[i])>0) 
      pi[i] = 90.0;
     else 
      pi[i] = -90.0;
    }
    else
     pi[i] = atan(fi[i]/fr[i])*360.0/6.283185306;
   }
}
} 
/* n = 2048   k = 11 */
void fft(float pr[], float pi[], int n, int k, float fr[], float fi[])
{
	int l =0,il = 0;
int it,m,is,i,j,nv,l0;
float p,q,s,vr,vi,poddr,poddi;

for (it=0; it<=n-1; it++)
{ 
   m = it; 
   is = 0;
   for(i=0; i<=k-1; i++)
   { 
    j = m/2; 
    is = 2*is+(m-2*j); 
    m = j;
   }
   fr[it] = pr[is]; 
   fi[it] = pi[is];
}
//----------------------------
pr[0] = 1.0; 
pi[0] = 0.0;
p = 6.283185306/(1.0*n);
pr[1] = cos(p); 
pi[1] = -sin(p);

if (l!=0) 
   pi[1]=-pi[1];

for (i=2; i<=n-1; i++)
{ 
   p = pr[i-1]*pr[1]; 
   q = pi[i-1]*pi[1];
   s = (pr[i-1]+pi[i-1])*(pr[1]+pi[1]);
   pr[i] = p-q; 
   pi[i] = s-p-q;
}

for (it=0; it<=n-2; it=it+2)
{ 
   vr = fr[it]; 
   vi = fi[it];
   fr[it] = vr+fr[it+1]; 
   fi[it] = vi+fi[it+1];
   fr[it+1] = vr-fr[it+1]; 
   fi[it+1] = vi-fi[it+1];
}
m = n/2; 
nv = 2;

for (l0=k-2; l0>=0; l0--)
{ 
   m = m/2; 
   nv = 2*nv;
   for(it=0; it<=(m-1)*nv; it=it+nv)
    for (j=0; j<=(nv/2)-1; j++)
    { 
     p = pr[m*j]*fr[it+j+nv/2];
     q = pi[m*j]*fi[it+j+nv/2];
     s = pr[m*j]+pi[m*j];
     s = s*(fr[it+j+nv/2]+fi[it+j+nv/2]);
     poddr = p-q; 
     poddi = s-p-q;
     fr[it+j+nv/2] = fr[it+j]-poddr;
     fi[it+j+nv/2] = fi[it+j]-poddi;
     fr[it+j] = fr[it+j]+poddr;
     fi[it+j] = fi[it+j]+poddi;
    }
}

if(l!=0)
{
   for(i=0; i<=n-1; i++)
   { 
    fr[i] = fr[i]/(1.0*n);
    fi[i] = fi[i]/(1.0*n);
   }
} 
if(il!=0)
{
   for(i=0; i<=n-1; i++)
   { 
    pr[i] = sqrt(fr[i]*fr[i]+fi[i]*fi[i]);
    if(fabs(fr[i])<0.000001*fabs(fi[i]))
    { 
     if ((fi[i]*fr[i])>0) 
      pi[i] = 90.0;
     else 
      pi[i] = -90.0;
    }
    else
     pi[i] = atan(fi[i]/fr[i])*360.0/6.283185306;
   }
}
}


/**
  * @brief  傅立叶计算，可以计算基波，谐波.
  * @param  h:基波1 一次谐波2.
  * 		*fft:需要计算的数组
  * 		n:数组大小
  * 		*real:存放实部
  * 		*imag:存放虚部
  * @retval None.
  * @verify .
  */
/**
  * @brief  .
  * @param  h:
			n:点数
			fft:
			real:
			imag:
  * @retval None
  */
#define PAI 3.1415926535897932384626433832795028841971        
void fft_CalcHarmonic (int h, const float *fft, const int n,
				float *real, float *imag)
{
	int i;
	*real = 0;
	*imag = 0;
	for(i=0; i<n; i++){
		*real += fft[i] * cos(h*2*PAI*i/n);
		*imag += fft[i] * sin(h*2*PAI*i/n);
	}
}

/**
  * @brief  .
  * @param  n:点数
			fft:AD值
			real:实部
			imag:虚部
			ang:幅度
			fm:半幅值
  * @retval None
  */
void fft_Calculate (int h, const float *fft, const int n,
				float *real, float *imag, float *ang, float *fm)
{
	int i;
	*real = 0;
	*imag = 0;
	for(i=0; i<n; i++){
		*real += fft[i] * cos(2*PAI*i/n);
		*imag += fft[i] * sin(2*PAI*i/n);
	}
	*ang = atan2 (*imag, *real);
	*fm = sqrt((*real)*(*real) + (*imag)*(*imag));
}
void THD1(float xreal [], float ximag [], float *thd)
{
	int i;
	double value = 0;
	double value1 = 0;
	value1 = sqrt(xreal[1]*xreal[1] + ximag[1]*ximag[1]);
	for(i=2; i<10; i++){
		value += sqrt(xreal[i]*xreal[i] + ximag[i]*ximag[i]);
	}
	*thd = value/value1;
}
