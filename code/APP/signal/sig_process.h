/*========================================================================
#   FileName: sig_process.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-31 15:15:43
========================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIG_PROCESS_H
#define __SIG_PROCESS_H
#include <stdint.h>
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
// typedef enum{
// 	SIGCHN_B,		//U26  正对地电压，交流电流
// 	SIGCHN_A,		//U30  负对地电压，交流电压
// }SIGCHN_n;
typedef enum{//零偏和误差系数
	SIGCHN_RZ=0,
	SIGCHN_RF,
	SIGCHN_CC,
	SIGCHN_VV,
}SIGCHN_EOR;
#define DEFAULT_PERIOD 4096//8个周期256点
typedef struct{
	volatile uint32_t status;
	volatile int32_t SIG_ZERODRIFT[4];//value 零偏(是AD值)
	volatile int32_t SIG_ERRCOE_GEZ;	//母线正电压修正系数 Div1000
	volatile int32_t SIG_ERRCOE_GEF;	//母线负电压修正系数
	volatile int32_t SIG_ERRCOE_RZ;		//R正对地系数
	volatile int32_t SIG_ERRCOE_RF;
	volatile int32_t SIG_ERRCOE_CP;		//电容修正系数
	volatile int32_t SIG_ERRCOE_CR;		//系统容抗修正系数
	volatile int32_t SIG_ERRCOE_RR;		//系统阻抗修正系数
	volatile int32_t SIG_ERRCOE_CAP[3];	//0:1x 1:51x 2:501x的修正系数
										//和容抗共用
	volatile int32_t SIG_SERIES_CAP[3];	//串联电容大小  在零偏校准时候
										//将三个输出端子短路.后台零偏校准
	//volatile int32_t SIG_ERRCOE[4];	//error coefficient
									//value div 1000 误差系数
	//volatile int32_t SIG_ZERO_CP;		//电容零偏
	//volatile int32_t SIG_PERIOD;	//PERIOD collect num
	//volatile double sigchnvalue[2];	//effective ad value
	volatile uint16_t collect_counter;//present collect counter

	volatile float GE_VOL;		//母线电压V
	volatile float ZR_RES;		//正对地电阻（千欧）
	volatile float FR_RES;		//负对地电阻（千欧）
	volatile float CP_CAP;		//对地电容
	volatile float CR_SYS;		//系统容抗
	volatile float RR_SYS;		//系统阻抗
}SIGPRC_StructType;

extern SIGPRC_StructType SIGPRC_Struct;
/* Exported constants --------------------------------------------------------*/

extern uint16_t sig_ad[DEFAULT_PERIOD][2];
/* Exported define -----------------------------------------------------------*/
#define SIG_CUR_RESAMP 1000.0//取样电流的电阻
#define SIG_VOL_RESAMP 21.0//取样电压  ---200K---10K----. 
/* 档位放大倍数 */
#define SIG_CAP1X_AMP 1.0
#define SIG_CAP2X_AMP 20.1204588
#define SIG_CAP3X_AMP 192.204588
#define SIG_STAT_IDLE (1<<0)
#define SIG_STAT_READYCONVER (1<<26)
#define SIG_STAT_COLING (1<<1)
#define SIG_STAT_COMPLETE (1<<2)
#define SIG_STAT_ERCLG	(1<<3)//对地电阻采集 标志被设置，表示正在进行
#define SIG_STAT_ERCLG_GE (1<<4)//母线电压采集
#define SIG_STAT_ERCLG_RZ (1<<5)//正对地电压采集
#define SIG_STAT_ERCLG_FZ (1<<6)//负对地电压采集,这个采集完毕进行对地电阻计算
#define SIG_STAT_ECCLG (1<<7)//对地容抗采集 标志被设置，表示正在进行
#define SIG_STAT_IMPCLG (1<<8)//系统容抗采集
//#define SIG_STAT_ECCLG (1<<9)//对地电容采集 标志被设置,表示正在进行对地电容采集
#define SIG_STAT_ECCLG_GE (1<<10)//
#define SIG_STAT_ECCLG_RZ (1<<11)
#define SIG_STAT_ECCLG_RF (1<<12)
#define SIG_STAT_ECCLG_CP (1<<13)
#define SIG_STAT_ECCLG_CP_OK (1<<14)//在容抗测量成功后置位
#define SIG_STAT_ECCLG_CP_FIRST (1<<15)//第一次容抗测量转换
#define SIG_STAT_EARTHLINE (1<<16)//地线未接标志
#define SIG_STAT_ERSCH (1<<17)//环网查找标志
#define SIG_STAT_ERSCH_DATA (1<<18)//环网阻抗容抗测量
#define SIG_STAT_ERSCH_COM (1<<19)//同步手持起
#define SIG_STAT_ERSCH_SERCH (1<<20)//准备查找，发送阻抗容抗
#define SIG_STAT_ERSCH_SERCHING (1<<22)//正在查找，间隔一秒发送同步
#define SIG_STAT_ERSCH_STOP (1<<21) //停止查找，发送结束数据包
#define SIG_STAT_CANCEL (1<<26) //测量中途取消
#define SIG_STAT_MEASURING (1<<23)//表示正在测量
#define SIG_STAT_MEASUREOK (1<<24)//表示测量完成
#define SIG_STAT_READY (1<<25)//表示准备就绪
#define SIGCHNAL_DA 0
#define SIGCHNAL_DB 1
#define SIGCHNAL_FVOL SIGCHNAL_DA
#define SIGCHNAL_VVOL SIGCHNAL_DA
#define SIGCHNAL_ZVOL SIGCHNAL_DB
#define SIGCHNAL_CVOL SIGCHNAL_DB
#define SIG_DEFAULT_FREQ 5
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void SIG_Init (void);
extern void SIG_CollectPeriod (void);
extern void SIG_SetCollectInterval (uint32_t value);
extern void SIG_ZeroDriftDetection (void);
extern void SIG_SaveSystemParam (void);
extern void SIG_GetSystemParam (void);
extern void SIG_CollectResisToEarth (void);
extern void SIG_CollectGeneratrix (void);
extern void SIG_SystemParamInit (void);
extern double SIG_GetZ_DCVol (void);
extern double SIG_GetF_DCVol (void);
extern double SIG_GetCurrent (void);
extern double SIG_GetVoltage (void);
extern void SIG_StartConverCollect (void);
extern void SIG_StopConverCollect (void);
extern void SIG_CalculateDC (void);
extern void SIG_GetAD7687data (uint16_t *ad);
extern void SIG_StartConverCollect (void);
extern void SIG_CompleteConverCollect (void);
extern void SIG_Thread_Sample (void);
extern void SIG_StartConverEarthResCollect (void);
extern void SIG_StopConverEarthResCollect (void);
extern void SIG_ZeroDiff (void);//四个通道零偏校准
extern void SIG_StopConverEarthCapCollect (void);
extern void SIG_StartConverEarthCapCollect (void);
extern void SIG_StopConverEarthSchCollect (void);
#endif 
/*********************************END OF FILE**********************************/


