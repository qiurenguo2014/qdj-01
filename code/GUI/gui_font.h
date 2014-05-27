/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_FONT_H
#define __GUI_FONT_H
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "GUI_BASIC.H"
/* Exported types ------------------------------------------------------------*/
typedef enum{
	FONT_HZK_DEFAULT=0,//
	FONT_HZK_16,
	FONT_HZK_24,
	FONT_HZK_32,
	FONT_HZK_COUNTER,
}FONT_HZK_TYPE;
typedef enum{
	FONT_ASC_DEFAULT =0,
	FONT_ASC_16,
	FONT_ASC_24,
	FONT_ASC_32,
	FONT_ASC_COUNTER,
}FONT_ASC_TYPE;
typedef enum{
	FONT_TTF_DEFAULT = 0,
	FONT_TTF_COUNTER,
}FONT_TTF;
#define FONT_WIDTH 0
#define FONT_HIGHT 1
typedef struct{
	uint32_t hzkfontsize[FONT_HZK_COUNTER];	//字模大小
	uint32_t ascfontsize[FONT_ASC_COUNTER];	//字模大小
}Font_StructTypedef;
typedef struct{
	FONT_TTF ttf;
	FONT_HZK_TYPE hzksize;
	FONT_ASC_TYPE ascsize;
}FONT_StructType;
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define FONT_ASC_16_HIGH 16
#define FONT_ASC_16_WITH 8
#define FONT_ASC_16_SIZE 16
#define FONT_ASC_24_HIGH 24
#define FONT_ASC_24_WITH 12
#define FONT_ASC_24_SIZE 48
#define FONT_ASC_32_HIGH 32
#define FONT_ASC_32_WITH 16
#define FONT_ASC_32_SIZE 64
#define FONT_HZK_16_HIGH 16
#define FONT_HZK_16_WITH 16
#define FONT_HZK_16_SIZE 32
#define FONT_HZK_24_HIGH 24
#define FONT_HZK_24_WITH 24
#define FONT_HZK_24_SIZE 72
#define FONT_HZK_32_HIGH 32
#define FONT_HZK_32_WITH 32
#define FONT_HZK_32_SIZE 128
extern FONT_StructType current_font;//当前字体
extern Font_StructTypedef Font_Struct;
extern PointXY COOR;
/* Exported variables --------------------------------------------------------*/
extern char printf_buf[100];
/* Exported functions prototypes ---------------------------------------------*/
extern void GUI_Font_Init (void);
extern void GUI_SetHzkType (FONT_HZK_TYPE hzktype);
extern void GUI_SetAscType (FONT_ASC_TYPE asctype);
extern void GUI_PutString (const uint16_t x, const uint16_t y, 
						const char *ptr);
extern void GUI_PutStringBack (const uint16_t x, const uint16_t y,
						const char *ptr);
extern void GUI_Printf (const char *buf);
extern void GUI_PrintfBack (const char *buf);
extern void GUI_PrintfBack_Rect (const char *buf);
extern void GUI_PutStringBack_Rect (const uint16_t x, const uint16_t y, const char *buf);
#endif 

/*********************************END OF FILE**********************************/

