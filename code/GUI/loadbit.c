/*************************************************************************
#    FileName: loadbit.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: ��ʾͼƬ
#  LastChange: 2013-11-29 09:23:48
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "lcddrive.h"
#include "gui_stockc.h"
#include "gui_basic.h"
#include "loadbit.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------*/
/**
  * @brief  ��ʾ����ͼƬ
  * 		��ʽ:ǰ�����ǿ�(0)�͸�(1).
  * 		0������ɫ
  * 		1��ǰ��ɫ
  * 		���ϵ��»�ͼ
  * @param  None.
  * @retval None.
  * @verify .
  */
void  GUI_LoadCharStr(uint32_t x, uint32_t y, uint8_t *dat)
{
	uint32_t i=2;
	PointXY coor;
	coor.y = y;
	coor.hight = dat[1];
	for(; coor.hight>0; coor.hight--){
		coor.with = dat[0];
		coor.x = x;
		for(; coor.with>0; coor.with--){
			if(dat[i]){
				GUI_Point(coor.x, coor.y, disp_color);
			}else{
				GUI_Point(coor.x, coor.y, back_color);
			}
			i++;
			coor.x++;
		}
		coor.y++;
	}
}






