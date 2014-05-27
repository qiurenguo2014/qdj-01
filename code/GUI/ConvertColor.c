/*************************************************************************
#    FileName: ConvertColor.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "ConvertColor.h"
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
uint16_t  GUI_Color2Index_565(uint32_t colorRGB) 
{  uint8_t  r, g, b;

   b = ( colorRGB>>(0+3) ) & 0x1f;		// 取B色的高5位
   g = ( colorRGB>>(8+2) ) & 0x3f;		// 取G色的高6位
   r = ( colorRGB>>(16+3)) & 0x1f;		// 取R色的高5位
   
   return( (r<<11) + (g<<5) + (b<<0) );		
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint32_t  GUI_Index2Color_565(uint16_t index) 
{  uint32_t  r, g, b;

   b = (index>>0)  & 0x1f;
   g = (index>>5)  & 0x3f;
   r = (index>>11) & 0x1f;
   r = r * 255 / 31;
   g = g * 255 / 63;
   b = b * 255 / 31;
   
   return( (r<<16) + (g<<8) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint16_t  GUI_Color2Index_555(uint32_t colorRGB) 
{  uint8_t  r, g, b;

   b = ( colorRGB>>(0+3) ) & 0x1f;
   g = ( colorRGB>>(8+3) ) & 0x1f;
   r = ( colorRGB>>(16+3)) & 0x1f;
  
   return( (r<<10) + (g<<5) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint32_t  GUI_Index2Color_555(uint16_t index) 
{  uint32_t  r, g, b;

   b = (index>>0)  & 0x1f;
   g = (index>>5)  & 0x1f;
   r = (index>>10) & 0x1f;
   r = r * 255 / 31;
   g = g * 255 / 31;
   b = b * 255 / 31;
   
   return( (r<<16) + (g<<8) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint16_t  GUI_Color2Index_444(uint32_t colorRGB) 
{  uint8_t r,g,b;

   b = ( colorRGB>>(0+4) ) & 0x0f;
   g = ( colorRGB>>(8+4) ) & 0x0f;
   r = ( colorRGB>>(16+4)) & 0x0f;
   
   return ( (r<<8) + (g << 4) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint32_t  GUI_Index2Color_444(uint16_t index) 
{  uint8_t  r,g,b;

   b = (index >> 0) & 0x0f;
   g = (index >> 4) & 0x0f;
   r = (index >> 8) & 0x0f;
  
   r = r * 17;
   g = g * 17;
   b = b * 17;
   
   return ( (r<<16) + (g<<8) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t  GUI_Color2Index_332(uint32_t colorRGB) 
{  uint32_t  r, g, b;

   b = (colorRGB>>0)  & 0xff;
   g = (colorRGB>>8)  & 0xff;
   r = (colorRGB>>16) & 0xff;
   r = (r * 7 + 127) / 255;
   g = (g * 7 + 127) / 255;
   b = (b + 42) / 85;
   
   return( (r<<5) + (g << 2) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint32_t  GUI_Index2Color_233(uint8_t index)
{  uint32_t  r, g, b;

   r = (index >> 5) * 255 / 7;
   g = ((index >> 3) & 7) * 255 / 7;
   b = (index & 3) * 85;
   
   return( (r<<16) + (g << 8) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t  GUI_Color2Index_222(uint32_t colorRGB)
{  uint32_t  r, g, b;

   b = (((colorRGB>>0) &255)+0x2a)/0x55;
   g = (((colorRGB>>8) &255)+0x2a)/0x55;
   r = (((colorRGB>>16)&255)+0x2a)/0x55;
   
   return( (r<<4) + (g<<2) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint32_t  GUI_Index2Color_222(uint8_t index) 
{  uint8_t  r, g, b;

   b = ((index>>0)&3) * 0x55;
   g = ((index>>2)&3) * 0x55;
   r = ((index>>4)&3) * 0x55;
   
   return( (r<<16) + (g<<8) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint8_t  GUI_Color2Index_111(uint32_t colorRGB) 
{  uint8_t  r, g, b;

   b = (colorRGB>>(0+7))  &1;
   g = (colorRGB>>(8+7))  &1;
   r = (colorRGB>>(16+7)) &1;
   
   return( (r<<2) + (g<<1) + (b<<0) );
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
uint32_t  GUI_Index2Color_111(uint8_t Index) 
{  uint8_t  r, g, b;

   b = ((Index>>0)&1) * 0xff;
   g = ((Index>>1)&1) * 0xff;
   r = ((Index>>2)&1) * 0xff;
   
   return( (r<<16) + (g<<8) + (b<<0) );
}



