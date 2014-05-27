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
#define MAX_FNAME_LEN 30	//��������󳤶ȣ�Ӧ������Ϊ��С����������ĳ��ȡ�	
#define MAX_PARM 10		//���Ϊ10������ ,�޸Ĵ˲���,�����޸�usmart_exe��֮��Ӧ.
#define PARM_LEN 200	//���в���֮�͵ĳ��Ȳ�����PARM_LEN���ֽ�,ע�⴮�ڽ��ղ���Ҫ��֮��Ӧ(��С��PARM_LEN)
#define MAX_COMMON 5
/* Exported constants --------------------------------------------------*/
//����������Ϣ�ṹ��
struct UARTSHELL_Info
{
	u8 Type[MAX_PARM];    	//���� 1 ���� 2 �ַ���
	u32 ParmData[MAX_PARM]; //������Ϣ:����������Ͳ������Ų�����ֵ��������ַ����Ͳ������Ų�����ʼ��ַ(ָ��ParmBuf)
	u8 OffSet[MAX_PARM];  	//�������ַ�����ƫ��
	u8 Long[MAX_PARM];    	//�����ĳ���
	u8 ParmBuf[PARM_LEN];	//�ַ����Ͳ�������ڴ���
};
//�������б�	 
struct UARTSHELL_FunTab
{
	void* func;			//����ָ��
	const u8* name;		//������(���Ҵ�)	 
};
//CHS_SHELL������
struct UARTSHELL_DEV
{
	const struct UARTSHELL_FunTab *funs;	//������ָ��
	u32 cnum;
	u16 USART_STAT;
	u8 fnum; //��������
	u8 pnum; //��������
	u8 id;   //����ID��
	u8 USART_BUF[MAX_FNAME_LEN+PARM_LEN];         //���ڽ��ջ�����
	struct UARTSHELL_Info PInfo;                          //������Ϣ�ṹ��
};
extern struct UARTSHELL_DEV SHL_Dev;				//��usmart_config.c���涨��
/* Exported define -----------------------------------------------------*/

/* Exported variables --------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------*/

extern void SHL_ProcessCh(u8 ch);
extern u8 SHL_Scan(void);		
extern void SHL_Init(void);
#endif 
/*********************************END OF FILE****************************/

