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
// 	SIGCHN_B,		//U26  ���Եص�ѹ����������
// 	SIGCHN_A,		//U30  ���Եص�ѹ��������ѹ
// }SIGCHN_n;
typedef enum{//��ƫ�����ϵ��
	SIGCHN_RZ=0,
	SIGCHN_RF,
	SIGCHN_CC,
	SIGCHN_VV,
}SIGCHN_EOR;
#define DEFAULT_PERIOD 4096//8������256��
typedef struct{
	volatile uint32_t status;
	volatile int32_t SIG_ZERODRIFT[4];//value ��ƫ(��ADֵ)
	volatile int32_t SIG_ERRCOE_GEZ;	//ĸ������ѹ����ϵ�� Div1000
	volatile int32_t SIG_ERRCOE_GEF;	//ĸ�߸���ѹ����ϵ��
	volatile int32_t SIG_ERRCOE_RZ;		//R���Ե�ϵ��
	volatile int32_t SIG_ERRCOE_RF;
	volatile int32_t SIG_ERRCOE_CP;		//��������ϵ��
	volatile int32_t SIG_ERRCOE_CR;		//ϵͳ�ݿ�����ϵ��
	volatile int32_t SIG_ERRCOE_RR;		//ϵͳ�迹����ϵ��
	volatile int32_t SIG_ERRCOE_CAP[3];	//0:1x 1:51x 2:501x������ϵ��
										//���ݿ�����
	volatile int32_t SIG_SERIES_CAP[3];	//�������ݴ�С  ����ƫУ׼ʱ��
										//������������Ӷ�·.��̨��ƫУ׼
	//volatile int32_t SIG_ERRCOE[4];	//error coefficient
									//value div 1000 ���ϵ��
	//volatile int32_t SIG_ZERO_CP;		//������ƫ
	//volatile int32_t SIG_PERIOD;	//PERIOD collect num
	//volatile double sigchnvalue[2];	//effective ad value
	volatile uint16_t collect_counter;//present collect counter

	volatile float GE_VOL;		//ĸ�ߵ�ѹV
	volatile float ZR_RES;		//���Եص��裨ǧŷ��
	volatile float FR_RES;		//���Եص��裨ǧŷ��
	volatile float CP_CAP;		//�Եص���
	volatile float CR_SYS;		//ϵͳ�ݿ�
	volatile float RR_SYS;		//ϵͳ�迹
}SIGPRC_StructType;

extern SIGPRC_StructType SIGPRC_Struct;
/* Exported constants --------------------------------------------------------*/

extern uint16_t sig_ad[DEFAULT_PERIOD][2];
/* Exported define -----------------------------------------------------------*/
#define SIG_CUR_RESAMP 1000.0//ȡ�������ĵ���
#define SIG_VOL_RESAMP 21.0//ȡ����ѹ  ---200K---10K----. 
/* ��λ�Ŵ��� */
#define SIG_CAP1X_AMP 1.0
#define SIG_CAP2X_AMP 20.1204588
#define SIG_CAP3X_AMP 192.204588
#define SIG_STAT_IDLE (1<<0)
#define SIG_STAT_READYCONVER (1<<26)
#define SIG_STAT_COLING (1<<1)
#define SIG_STAT_COMPLETE (1<<2)
#define SIG_STAT_ERCLG	(1<<3)//�Եص���ɼ� ��־�����ã���ʾ���ڽ���
#define SIG_STAT_ERCLG_GE (1<<4)//ĸ�ߵ�ѹ�ɼ�
#define SIG_STAT_ERCLG_RZ (1<<5)//���Եص�ѹ�ɼ�
#define SIG_STAT_ERCLG_FZ (1<<6)//���Եص�ѹ�ɼ�,����ɼ���Ͻ��жԵص������
#define SIG_STAT_ECCLG (1<<7)//�Ե��ݿ��ɼ� ��־�����ã���ʾ���ڽ���
#define SIG_STAT_IMPCLG (1<<8)//ϵͳ�ݿ��ɼ�
//#define SIG_STAT_ECCLG (1<<9)//�Եص��ݲɼ� ��־������,��ʾ���ڽ��жԵص��ݲɼ�
#define SIG_STAT_ECCLG_GE (1<<10)//
#define SIG_STAT_ECCLG_RZ (1<<11)
#define SIG_STAT_ECCLG_RF (1<<12)
#define SIG_STAT_ECCLG_CP (1<<13)
#define SIG_STAT_ECCLG_CP_OK (1<<14)//���ݿ������ɹ�����λ
#define SIG_STAT_ECCLG_CP_FIRST (1<<15)//��һ���ݿ�����ת��
#define SIG_STAT_EARTHLINE (1<<16)//����δ�ӱ�־
#define SIG_STAT_ERSCH (1<<17)//�������ұ�־
#define SIG_STAT_ERSCH_DATA (1<<18)//�����迹�ݿ�����
#define SIG_STAT_ERSCH_COM (1<<19)//ͬ���ֳ���
#define SIG_STAT_ERSCH_SERCH (1<<20)//׼�����ң������迹�ݿ�
#define SIG_STAT_ERSCH_SERCHING (1<<22)//���ڲ��ң����һ�뷢��ͬ��
#define SIG_STAT_ERSCH_STOP (1<<21) //ֹͣ���ң����ͽ������ݰ�
#define SIG_STAT_CANCEL (1<<26) //������;ȡ��
#define SIG_STAT_MEASURING (1<<23)//��ʾ���ڲ���
#define SIG_STAT_MEASUREOK (1<<24)//��ʾ�������
#define SIG_STAT_READY (1<<25)//��ʾ׼������
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
extern void SIG_ZeroDiff (void);//�ĸ�ͨ����ƫУ׼
extern void SIG_StopConverEarthCapCollect (void);
extern void SIG_StartConverEarthCapCollect (void);
extern void SIG_StopConverEarthSchCollect (void);
#endif 
/*********************************END OF FILE**********************************/


