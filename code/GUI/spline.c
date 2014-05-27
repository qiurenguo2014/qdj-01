/*************************************************************************
#    FileName: spline.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/

#include "math.h"
#include "spline.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
int  max(int a, int b)
{  if(a>b) return(a);
     else  return(b);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
int  abs(int a)
{  if(a>0) return(a);
     else  return(-a);
}

/* 曲线类变量及相关函数 */
typedef  struct
{  float  	Ax, Ay;			// 定义A、B、C点坐标变量
   float  	Bx, By;
   float  	Cx, Cy;
   
   int    	Ndiv;			// 定义曲线平滑参数变量     
} CURVE;  

#define  DIV_FACTOR 		8.0				/* 平滑控制参数 */
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  CURVE_SetCurve(CURVE *cl,
                     float ax, float ay, 
               		 float bx, float by,
               		 float cx, float cy) 
{  cl->Ax = ax;
   cl->Ay = ay;
   cl->Bx = bx; 
   cl->By = by;
   cl->Cx = cx; 
   cl->Cy = cy;
   
   cl->Ndiv = (int)(max( abs((int)ax), abs((int)ay) ) / DIV_FACTOR);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
int  CURVE_GetCount(CURVE *cl)
{  if(0 == cl->Ndiv) cl->Ndiv=1;

   return(cl->Ndiv+1);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  CURVE_GetCurve(CURVE *cl, float x, float y, PointXY points[], int *PointCount)
{  int    X, Y;
   float  t, f, g, h;
   int    i;

   if(cl->Ndiv==0)  cl->Ndiv = 1;

    /* 新增一个点到结构 */
	X = (int)x; 
	Y = (int)y;
	points[*PointCount].x = X;
	points[*PointCount].y = Y;
	(*PointCount)++;

    /* 变换出ndiv个点 */
	for(i=1; i<=cl->Ndiv; i++)
	{  t = 1.0f / (float)cl->Ndiv * (float)i;
	   f = t * t * (3.0f - 2.0f * t);
	   g = t * (t - 1.0f) * (t-1.0f);
	   h = t * t * (t-1.0f);
		
	   X = (int)(x + cl->Ax*f + cl->Bx*g + cl->Cx*h);
	   Y = (int)(y + cl->Ay*f + cl->By*g + cl->Cy*h);
		
	   points[*PointCount].x = X;
	   points[*PointCount].y = Y;
	   (*PointCount)++;
	}
}
/***************************************************************************/
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  SPLINE_SetSpline(SPLINE *sl, PointXY pt[], int np)
{  int  i;

   sl->Np = np;

   /* 将点数据复制到sl对象 */
   for(i=0; i<sl->Np; i++) 
   {  sl->Px[i] = (float)pt[i].x;  
	  sl->Py[i] = (float)pt[i].y;
   }
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  SPLINE_MatrixSolve(SPLINE *sl, float *Bb)
{  float  Work[NPMAX];
   float  WorkB[NPMAX];
   int    i, j;
		
   for(i=0; i<=(sl->Np-1); i++) 
   {  Work[i] = Bb[i] / sl->Mat[1][i];
	  WorkB[i] = Work[i];
   }

   for(j=0; j<10; j++) 
   {  Work[0] = (Bb[0] - sl->Mat[2][0] * WorkB[1]) / sl->Mat[1][0];
	  for(i=1; i<(sl->Np-1); i++ ) 
  	  {  Work[i] = (Bb[i] - sl->Mat[0][i] * WorkB[i-1] - sl->Mat[2][i] * WorkB[i+1]) / sl->Mat[1][i];
	  }
	  Work[sl->Np-1] = (Bb[sl->Np-1] - sl->Mat[0][sl->Np-1] * WorkB[sl->Np-2]) / sl->Mat[1][sl->Np-1];

	  for(i=0; i<=(sl->Np-1); i++) 
	  {  WorkB[i] = Work[i];
	  }
   }
   
   for(i=0; i<=(sl->Np-1); i++) 
   {  Bb[i] = Work[i];
   }
   
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  SPLINE_Generate(SPLINE *sl) 
{  float  k[NPMAX];
   float  AMag , AMagOld;
   int    i;

   /* 设置A点值 */
   for(i=0 ; i<=(sl->Np-2); i++ ) 
   {  sl->Ax[i] = sl->Px[i+1] - sl->Px[i];
	  sl->Ay[i] = sl->Py[i+1] - sl->Py[i];
   }
		
   /* 计算k变量 */
   AMagOld = (float)sqrt(sl->Ax[0] * sl->Ax[0] + sl->Ay[0] * sl->Ay[0]);
   for(i=0 ; i<=(sl->Np-3); i++) 
   {  AMag = (float)sqrt(sl->Ax[i+1] * sl->Ax[i+1] + sl->Ay[i+1] * sl->Ay[i+1]);
	  k[i] = AMagOld / AMag;
	  AMagOld = AMag;
   }
   k[sl->Np-2] = 1.0f;

   /* 矩阵计算 */
   for(i=1; i<=(sl->Np-2); i++) 
   {  sl->Mat[0][i] = 1.0f;
	  sl->Mat[1][i] = 2.0f * k[i-1] * (1.0f + k[i-1]);
	  sl->Mat[2][i] = k[i-1] * k[i-1] * k[i];
   }
   sl->Mat[1][0] = 2.0f;
   sl->Mat[2][0] = k[0];
   sl->Mat[0][sl->Np-1] = 1.0f;
   sl->Mat[1][sl->Np-1] = 2.0f * k[sl->Np-2];
 
   for(i=1; i<=(sl->Np-2);i++) 
   {  sl->Bx[i] = 3.0f*(sl->Ax[i-1] + k[i-1] * k[i-1] * sl->Ax[i]);
	  sl->By[i] = 3.0f*(sl->Ay[i-1] + k[i-1] * k[i-1] * sl->Ay[i]);
   }
   sl->Bx[0] = 3.0f * sl->Ax[0];
   sl->By[0] = 3.0f * sl->Ay[0];
   sl->Bx[sl->Np-1] = 3.0f * sl->Ax[sl->Np-2];
   sl->By[sl->Np-1] = 3.0f * sl->Ay[sl->Np-2];

   SPLINE_MatrixSolve(sl, sl->Bx);
   SPLINE_MatrixSolve(sl, sl->By);

   for(i=0 ; i<=sl->Np-2 ; i++ ) 
   {  sl->Cx[i] = k[i] * sl->Bx[i+1];
	  sl->Cy[i] = k[i] * sl->By[i+1];
   }
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
int  SPLINE_GetCurveCount(SPLINE *sl)
{  CURVE  curve_bak;
   int    count = 0;
   int    i;
   
   for(i=0; i<(sl->Np - 1); i++)	// 历遍所有特征点 
   {  /* 设置样条曲线(三点)curve_bak */
      CURVE_SetCurve(&curve_bak, 
                     sl->Ax[i], sl->Ay[i], 
                     sl->Bx[i], sl->By[i], 
                     sl->Cx[i], sl->Cy[i]);
      /* 取得曲线curve_bak上端点的个数 */
	  count += CURVE_GetCount(&curve_bak);
   }
   
   return(count);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  SPLINE_GetCurve(SPLINE *sl, PointXY points[], int *PointCount)
{  CURVE  curve_bak;
   int    i;

   *PointCount = 0;						// 初始化点计数器为0
   for(i=0; i<(sl->Np-1); i++) 			// 历遍所有特征点
   {  /* 设置样条曲线(三点)curve_bak */
   	  CURVE_SetCurve(&curve_bak, 
   					 sl->Ax[i], sl->Ay[i],
                     sl->Bx[i], sl->By[i], 
                     sl->Cx[i], sl->Cy[i]);
                     
      /* 取得样条曲线curve_bak的端点数据及个数 */               
	  CURVE_GetCurve(&curve_bak, 
	                 sl->Px[i], sl->Py[i], 
	                 points, 
	                 PointCount);
   }
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void  GUI_Spline(PointXY points[], int no, TCOLOR color)
{  SPLINE   sl;
   PointXY  sl_points[NPMAX * (int)DIV_FACTOR];
   
   if( (no<3)||(no>NPMAX) ) return;
   
   SPLINE_SetSpline(&sl, points, no);
   SPLINE_Generate(&sl);
   SPLINE_GetCurve(&sl, sl_points, &no);

   GUI_LineS((uint32_t *)sl_points, no, color);
}



	









