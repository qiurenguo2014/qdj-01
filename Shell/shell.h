/*************************************************************************
#    FileName: shell.h
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: 
#  LastChange: 2014-02-05 23:06:16
*************************************************************************/
/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __SHELL_H
#define __SHELL_H
/* Includes ------------------------------------------------------------*/
#include <stdio.h>
/* Exported types ------------------------------------------------------*/
typedef void (*fun) (void);
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define MAX_FNAME_LEN 30	//函数名最大长度，应该设置为不小于最长函数名的长度。	
#define MAX_PARM 10		//最大为10个参数 ,修改此参数,必须修改usmart_exe与之对应.
#define PARM_LEN 200	//所有参数之和的长度不超过PARM_LEN个字节,注意串口接收部分要与之对应(不小于PARM_LEN)
#define MAX_COMMON 5
/* Exported constants --------------------------------------------------*/
//函数参数信息结构体
struct UARTSHELL_Info
{
	u8 Type[MAX_PARM];    	//类型 1 数字 2 字符串
	u32 ParmData[MAX_PARM]; //参数信息:如果是数字型参数则存放参数的值，如果是字符串型参数则存放参数起始地址(指向ParmBuf)
	u8 OffSet[MAX_PARM];  	//参数在字符串中偏移
	u8 Long[MAX_PARM];    	//参数的长度
	u8 ParmBuf[PARM_LEN];	//字符串型参数存放内存区
};
//函数名列表	 
struct UARTSHELL_FunTab
{
	void* func;			//函数指针
	const u8* name;		//函数名(查找串)	 
};
//CHS_SHELL管理器
struct UARTSHELL_DEV
{
	const struct UARTSHELL_FunTab *funs;	//函数名指针
	u32 cnum;
	u16 USART_STAT;
	u8 fnum; //函数数量
	u8 pnum; //参数数量
	u8 id;   //函数ID号
	u8 USART_BUF[MAX_FNAME_LEN+PARM_LEN];         //串口接收缓冲区
	struct UARTSHELL_Info PInfo;                          //参数信息结构体
};
extern struct UARTSHELL_DEV SHL_Dev;				//在usmart_config.c里面定义
/* Exported define -----------------------------------------------------*/

/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/

extern void SHL_ProcessCh(u8 ch);
extern u8 SHL_Scan(void);		
extern void SHL_Init(void);
#endif 
/*********************************END OF FILE****************************/

