/*========================================================================
#   FileName: sig_process.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2014-01-09 10:00:23
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "../../common/datatype.h"
#include "../../bsp/systick/systick.h"
#include "../../bsp/flash/memory.h"
#include "../../devdriver/adg/adg.h"
#include "../../devdriver/ad7687/ad7687.h"
#include "../../devdriver/relay/relay.h"
#include "../../devdriver/tim/tim.h"
#include "../../devdriver/wfg/wave.h"
#include "../../gui/GUI_KEYBOARD.h"
#include "../../gui/gui_font.h"
#include "../../gui/widget.h"
#include "../wifi/protocol.h"
#include "fly.h"
#include "sig_process.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
SIGPRC_StructType SIGPRC_Struct;
//AD值  0：电流 1：电压  0:正极  1：负极
uint16_t sig_ad[DEFAULT_PERIOD][2];
/* Variables --------------------------------------------------------*/
/* Functions prototypes ---------------------------------------------*/
static void _SIG_Delay1ms (uint32_t delay);
double SIG_GetFVOLvalues (void);
double SIG_GetZVOLvalues (void);
double SIG_GetCVOLvalues (void);
double SIG_GetVVOLvalues (void);
void SIG_EarthResCollectContinus (void);
void SIG_EarthCapCollectContinus (void);
void SIG_CalcEarthRes (double uge, double urz, double urf);
void SIG_CalcEarthCap (void);
void SIG_Thread_Earthres (void);
void SIG_Thread_Earthcap (void);
void SIG_Thread_EarthSch (void);
/* Functions --------------------------------------------------------*/
static double s_value[3];
void SIG_Thread_Sample (void)
{
	if(SIGPRC_Struct.status & SIG_STAT_ERCLG){//对地电阻
		SIG_Thread_Earthres ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG){//对地电容
		SIG_Thread_Earthcap ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ERSCH){
		SIG_Thread_EarthSch ();
	}
}
/* 稳定系数 */
#define STABILITY_COE0 1.05
#define STABILITY_COE1 0.95
double back_value;
volatile uint32_t test_mvv;
long jiffies_conver;
uint32_t res_stat;
const uint32_t col_delay = 50;
void SIG_Thread_Earthres (void)
{
	if(SIGPRC_Struct.status & SIG_STAT_COMPLETE){
		/* 检测采集完成标志 */
		if(SIGPRC_Struct.status & SIG_STAT_ERCLG_GE){
			/* 获得母线电压 */
			s_value[0] = ((SIG_GetZVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ])*
					SIGPRC_Struct.SIG_ERRCOE_GEZ/1000.0);
			s_value[0] -= ((SIG_GetFVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF])*
					SIGPRC_Struct.SIG_ERRCOE_GEF/1000.0);
			back_value = 0;//防止影响下次测量
			SIG_EarthResCollectContinus ();
		}else if(SIGPRC_Struct.status & SIG_STAT_ERCLG_RZ){
			/* 获得正对地电压 */
			s_value[1] = (SIG_GetZVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ])*
					SIGPRC_Struct.SIG_ERRCOE_GEZ/1000.0;
			/* 等待电压稳定,消除电容放电干扰 */
			if (back_value == 0){
				back_value = s_value[1];
				STK_delay10ms (col_delay);
				SIG_StartConverCollect ();
			}else{
				if((back_value/s_value[1] > STABILITY_COE1) &&
						(back_value/s_value[1] < STABILITY_COE0)){
					SIG_EarthResCollectContinus ();
					back_value = 0;
				}else if((s_value[1]/back_value > STABILITY_COE1) &&
						(s_value[1]/back_value < STABILITY_COE0)){
					SIG_EarthResCollectContinus ();
					back_value = 0;
				}else{
					back_value = s_value[1];
					STK_delay10ms (col_delay);
					SIG_StartConverCollect ();
				}
			}
		}else if(SIGPRC_Struct.status & SIG_STAT_ERCLG_FZ){
			/* 获得负对地电压，并计算对地电阻 */
			s_value[2] = (SIG_GetFVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF])*
					SIGPRC_Struct.SIG_ERRCOE_GEF/1000.0;
			/* 等待电压稳定 */
			if (back_value == 0){
				back_value = s_value[2];
				STK_delay10ms (col_delay);
				SIG_StartConverCollect ();
			}else{
				if((back_value/s_value[2] > STABILITY_COE1 &&
						back_value/s_value[2] < STABILITY_COE0)){
					SIG_CalcEarthRes (s_value[0], s_value[1],
							s_value[2]);
					SIG_EarthResCollectContinus ();
					GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
					back_value = 0;
				}else if(s_value[2]/back_value > STABILITY_COE1 &&
						s_value[2]/back_value < STABILITY_COE0){
					SIG_CalcEarthRes (s_value[0], s_value[1], 
							s_value[2]);
					SIG_EarthResCollectContinus ();
					GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
					back_value = 0;
				}else{
					back_value = s_value[2];
					STK_delay10ms (col_delay);
					SIG_StartConverCollect ();
				}
			}
		}
	}
}
extern volatile uint8_t vk_msg;
void SIG_Thread_Earthcap (void)
{	
	if(SIGPRC_Struct.status & SIG_STAT_COMPLETE){
		//GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET,0);
		if(SIGPRC_Struct.status & SIG_STAT_ECCLG_GE){
			/* 获得母线电压 */
			s_value[0] = ((SIG_GetFVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF])*
					SIGPRC_Struct.SIG_ERRCOE_GEZ/1000.0);
			s_value[0] -= ((SIG_GetZVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ])*
					SIGPRC_Struct.SIG_ERRCOE_GEF/1000.0);
			back_value = 0;//防止影响下次测量
			SIG_EarthCapCollectContinus ();
		}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG_RZ){
			/* 获得正对地电压 */
			s_value[1] = (SIG_GetZVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ]);
			/* 等待电压稳定 */
			if (back_value == 0){
				back_value = s_value[1];
				STK_delay10ms (col_delay);
				SIG_StartConverCollect ();
			}else{
				if((back_value/s_value[1] > STABILITY_COE1) &&
						(back_value/s_value[1] < STABILITY_COE0)){
					SIG_EarthCapCollectContinus ();
					back_value = 0;
				}else if((s_value[1]/back_value > STABILITY_COE1) &&
						(s_value[1]/back_value < STABILITY_COE0)){
					SIG_EarthCapCollectContinus ();
					back_value = 0;
				}else{
					back_value = s_value[1];
					STK_delay10ms (col_delay);
					SIG_StartConverCollect ();
				}
			}
		}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG_RF){
			/* 获得负对地电压，并计算对地电阻 */
			s_value[2] = (SIG_GetFVOLvalues ()-
					SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF]);
			/* 等待电压稳定 */
			if (back_value == 0){
				back_value = s_value[2];
				STK_delay10ms (col_delay);
				SIG_StartConverCollect ();
			}else{
				if((back_value/s_value[2] > STABILITY_COE1 &&
						back_value/s_value[2] < STABILITY_COE0)){
					SIG_CalcEarthRes (s_value[0], s_value[1], 
							s_value[2]);
					_UM_EarthResShowValue ();
					SIG_EarthCapCollectContinus ();
					back_value = 0;
				}else if(s_value[2]/back_value > STABILITY_COE1 &&
						s_value[2]/back_value < STABILITY_COE0){
					SIG_CalcEarthRes (s_value[0], s_value[1], 
							s_value[2]);
					_UM_EarthResShowValue ();
					SIG_EarthCapCollectContinus ();
					back_value = 0;
				}else{
					back_value = s_value[2];
					STK_delay10ms (col_delay);
					SIG_StartConverCollect ();
				}
			}
		}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG_CP){
			/* 获得交流电压和电流  并傅立叶计算有效值 */
			SIG_CalcEarthCap ();
			if(SIGPRC_Struct.status & SIG_STAT_ECCLG_CP_OK){
				SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_CP_OK;
				SIG_EarthCapCollectContinus ();
				GUI_WidgetProcessService (WIDGET_UPDATA_FOCUSWIDGET, 0);
			}else{
				SIG_StartConverCollect ();
			}
		}
	}
}
#include "../wifi/protocol.h"
extern const TITLE value[7];
long sch_delay;
uint8_t sch_dot;
void SIG_Thread_EarthSch (void)
{
	if(SIGPRC_Struct.status & SIG_STAT_ERSCH_DATA){
		if(SIGPRC_Struct.status & SIG_STAT_COMPLETE){
			/* 获得交流电压和电流  并傅立叶计算有效值 */
			SIG_CalcEarthCap ();
			if(SIGPRC_Struct.status & SIG_STAT_ECCLG_CP_OK){
				SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_CP_OK;
				SIGPRC_Struct.status &= ~SIG_STAT_ERSCH_DATA;
				if(SIGPRC_Struct.status & SIG_STAT_EARTHLINE){
					SIG_StopConverEarthSchCollect ();
				}else{
					SIGPRC_Struct.status |= SIG_STAT_ERSCH_COM;
				}
				GUI_WidgetProcessService (WIDGET_UPDATA_DATA, 0);
				sch_delay = jiffies+100;
				sch_dot = 0;
			}else{
				SIG_StartConverCollect ();
			}
		}
	}else if(SIGPRC_Struct.status & SIG_STAT_ERSCH_COM){
		GUI_SetAscType (FONT_ASC_16);
		GUI_SetHzkType (FONT_HZK_16);
		GUI_SetColor (BLACK, LBBLUE);
		if(sch_dot == 0){
			GUI_PutStringBack (50,112,
						"请打开手持器.    ");
		}else if(sch_dot == 1){
			GUI_PutStringBack (50,112,
						"请打开手持器..   ");
		}else if(sch_dot == 2){
			GUI_PutStringBack (50,112,
						"请打开手持器...  ");
		}else if(sch_dot == 3){
			GUI_PutStringBack (50,112,
						"请打开手持器.... ");
		}else if(sch_dot == 4){
			GUI_PutStringBack (50,112,
						"请打开手持器.....");
		}else if(sch_dot == 5){
		}
		if(sch_delay <= jiffies){
			if(sch_dot == 4){
				sch_dot = 0;
			}else{
				sch_dot++;
			}
			sch_delay = jiffies + 100;
		}
		if(Protocol_CheckSlaveOnline() == TRUE){
			GUI_PutStringBack (50,112,
						"请打开手持器.....OK!");
			GUI_PutStringBack (50,132,
						"正在发送数据.");
			SIGPRC_Struct.status |= SIG_STAT_ERSCH_SERCH;
			SIGPRC_Struct.status &= ~SIG_STAT_ERSCH_COM;
		}
	}else if(SIGPRC_Struct.status & SIG_STAT_ERSCH_SERCH){
		ReactParam rect;
		GUI_SetAscType (FONT_ASC_16);
		GUI_SetHzkType (FONT_HZK_16);
		GUI_SetColor (BLACK, LBBLUE);
		if(sch_dot == 0){
			GUI_PutStringBack (50,132,
						"正在发送数据.    ");
		}else if(sch_dot == 1){
			GUI_PutStringBack (50,132,
						"正在发送数据..   ");
		}else if(sch_dot == 2){
			GUI_PutStringBack (50,132,
						"正在发送数据...  ");
		}else if(sch_dot == 3){
			GUI_PutStringBack (50,132,
						"正在发送数据.... ");
		}else if(sch_dot == 4){
			GUI_PutStringBack (50,132,
						"正在发送数据.....");
		}else if(sch_dot == 5){
		}
		if(sch_delay <= jiffies){
			if(sch_dot == 4){
				sch_dot = 0;
			}else{
				sch_dot++;
			}
			sch_delay = jiffies + 100;
		}
		rect.CapReact = (uint16_t)(SIGPRC_Struct.CR_SYS*10);
		rect.ResReact = (uint16_t)(SIGPRC_Struct.RR_SYS*10);
		if(Protocol_SendReactParams(&rect) == TRUE){
			SIGPRC_Struct.status |= SIG_STAT_ERSCH_SERCHING;
			SIGPRC_Struct.status &= ~SIG_STAT_ERSCH_SERCH;
			GUI_PutStringBack (50,132,
						"正在发送数据.....OK!");
			sch_dot = 0;
			sch_delay = jiffies + 200;
		}
	}else if(SIGPRC_Struct.status & SIG_STAT_ERSCH_SERCHING){
		if(sch_delay <= jiffies){
			Protocol_CheckSlaveOnline();
//			if(Protocol_CheckSlaveOnline()== TRUE){
//				sch_dot = 0;
//			}else{
//				sch_dot++;
//			}
//			if(sch_dot>5){
//				SIG_StopConverEarthSchCollect();
//				GUI_WidgetProcessService(WIDGET_UPDATA_FOCUSWIDGET,0);
//			}
			sch_delay = jiffies + 6000;
		}else{
		
		}
	}
}
void SIG_Init(void)
{
	SIGPRC_Struct.status = SIG_STAT_IDLE;
	if (MM_ReadLWord (OFFSET_SYSTEM)&SYSPARAM_INIT){
		SIG_SystemParamInit();
	}else{
		SIG_GetSystemParam ();
	}
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SIG_SystemParamInit (void)
{
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ] = 0;
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF] = 0;
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_CC] = 0;
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_VV] = 0;
	SIGPRC_Struct.SIG_ERRCOE_GEZ = 1000;
	SIGPRC_Struct.SIG_ERRCOE_GEF = 1000;
	SIGPRC_Struct.SIG_ERRCOE_RZ = 1000;
	SIGPRC_Struct.SIG_ERRCOE_RF = 1000;
	SIGPRC_Struct.SIG_ERRCOE_CP = 1000;
	SIGPRC_Struct.SIG_ERRCOE_RR = 1000;
	SIGPRC_Struct.SIG_ERRCOE_CR = 1000;
	SIGPRC_Struct.SIG_ERRCOE_CAP[0] = 1000;
	SIGPRC_Struct.SIG_ERRCOE_CAP[1] = 1000;
	SIGPRC_Struct.SIG_ERRCOE_CAP[2] = 1000;
	SIGPRC_Struct.SIG_SERIES_CAP[2] = 1000;//uf
	SIGPRC_Struct.SIG_SERIES_CAP[1] = 1000;//uf
	SIGPRC_Struct.SIG_SERIES_CAP[0] = 1000;//uf
	SIG_SaveSystemParam ();
}
void SIG_GetSystemParam (void)
{
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ] = 
		MM_ReadLWord (OFFSET_SIGCHN_RZ_ZERO);
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF] = 
		MM_ReadLWord (OFFSET_SIGCHN_RF_ZERO);
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_CC] = 
		MM_ReadLWord (OFFSET_SIGCHN_CC_ZERO);
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_VV] = 
		MM_ReadLWord (OFFSET_SIGCHN_VV_ZERO);
	SIGPRC_Struct.SIG_ERRCOE_GEZ =
		MM_ReadLWord (OFFSET_SIGCHN_GEZ_ERR);
	SIGPRC_Struct.SIG_ERRCOE_GEF =
		MM_ReadLWord (OFFSET_SIGCHN_GEF_ERR);
	SIGPRC_Struct.SIG_ERRCOE_RZ =
		MM_ReadLWord (OFFSET_SIGCHN_RZ_ERR);
	SIGPRC_Struct.SIG_ERRCOE_RF =
		MM_ReadLWord (OFFSET_SIGCHN_RF_ERR);
	SIGPRC_Struct.SIG_ERRCOE_CP =
		MM_ReadLWord (OFFSET_SIGCHN_CP_ERR);
	SIGPRC_Struct.SIG_ERRCOE_CR =
		MM_ReadLWord (OFFSET_SIGCHN_CR_ERR);
	SIGPRC_Struct.SIG_ERRCOE_RR =
		MM_ReadLWord (OFFSET_SIGCHN_RR_ERR);
	SIGPRC_Struct.SIG_ERRCOE_CAP[0] =
		MM_ReadLWord (OFFSET_CAP1X_ERR);
	SIGPRC_Struct.SIG_ERRCOE_CAP[1] =
		MM_ReadLWord (OFFSET_CAP2X_ERR);
	SIGPRC_Struct.SIG_ERRCOE_CAP[2] =
		MM_ReadLWord (OFFSET_CAP3X_ERR);
	SIGPRC_Struct.SIG_SERIES_CAP[0] =
		MM_ReadLWord (OFFSET_SERIES_CAP1x);
	SIGPRC_Struct.SIG_SERIES_CAP[1] =
		MM_ReadLWord (OFFSET_SERIES_CAP2x);
	SIGPRC_Struct.SIG_SERIES_CAP[2] =
		MM_ReadLWord (OFFSET_SERIES_CAP3x);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void SIG_SaveSystemParam (void)
{
	MM_ProgramLWord (OFFSET_SIGCHN_RZ_ZERO,
			SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ]);
	MM_ProgramLWord (OFFSET_SIGCHN_RF_ZERO,
			SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF]);
	MM_ProgramLWord (OFFSET_SIGCHN_CC_ZERO,
			SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_CC]);
	MM_ProgramLWord (OFFSET_SIGCHN_VV_ZERO,
			SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_VV]);

	MM_ProgramLWord (OFFSET_SIGCHN_GEZ_ERR,
			SIGPRC_Struct.SIG_ERRCOE_GEZ);
	MM_ProgramLWord (OFFSET_SIGCHN_GEF_ERR,
			SIGPRC_Struct.SIG_ERRCOE_GEF);
	MM_ProgramLWord (OFFSET_SIGCHN_RZ_ERR,
			SIGPRC_Struct.SIG_ERRCOE_RZ);
	MM_ProgramLWord (OFFSET_SIGCHN_RF_ERR,
			SIGPRC_Struct.SIG_ERRCOE_RF);
	MM_ProgramLWord (OFFSET_SIGCHN_CP_ERR,
			SIGPRC_Struct.SIG_ERRCOE_CP);
	MM_ProgramLWord (OFFSET_SIGCHN_CR_ERR,
			SIGPRC_Struct.SIG_ERRCOE_CR);
	MM_ProgramLWord (OFFSET_SIGCHN_RR_ERR,
			SIGPRC_Struct.SIG_ERRCOE_RR);
	MM_ProgramLWord (OFFSET_CAP1X_ERR,
			SIGPRC_Struct.SIG_ERRCOE_CAP[0]);
	MM_ProgramLWord (OFFSET_CAP2X_ERR,
			SIGPRC_Struct.SIG_ERRCOE_CAP[1]);
	MM_ProgramLWord (OFFSET_CAP3X_ERR,
			SIGPRC_Struct.SIG_ERRCOE_CAP[2]);
	MM_ProgramLWord (OFFSET_SERIES_CAP1x,
			SIGPRC_Struct.SIG_SERIES_CAP[0]);
	MM_ProgramLWord (OFFSET_SERIES_CAP2x,
			SIGPRC_Struct.SIG_SERIES_CAP[1]);
	MM_ProgramLWord (OFFSET_SERIES_CAP3x,
			SIGPRC_Struct.SIG_SERIES_CAP[2]);
	MM_ClrSysFlag (SYSPARAM_INIT);
}
/**
  * @brief      U1/Rf+U1/R1 = U2/Rz+U2/R2
  *             U3/Rz+U1/R1 = U4/Rf+U4/R2
  *
  *	1/Rf = [U2(U3+U4)/R2-U3(U1+U2)/R1]/(U1U3-U2U4) 
  *             1/Rz = [U4(U1+U2)/R2-U1(U3+U4)/R1]/(U1U3-U2U4).
  * @param  None
  * @retval None
  */
double SIG_GetFVOLvalues (void)
{
	double valuef = 0;
	uint16_t i;
	for(i=0; i<DEFAULT_PERIOD; i++){
		valuef += (int16_t)sig_ad[i][1];
	}
	valuef /= (double)DEFAULT_PERIOD;
	return valuef;
}
double SIG_GetZVOLvalues (void)
{
	double valuef = 0;
	uint16_t i;
	for(i=0; i<DEFAULT_PERIOD; i++){
		valuef += (int16_t)sig_ad[i][0];
	}
	valuef /= (double)DEFAULT_PERIOD;
	return valuef;
}
void SIG_PrintfADvalue(void)
{
	uint16_t i;
	printf ("\r\nvol=[");
	for(i=0; i<DEFAULT_PERIOD; i++){
		printf("%d,",(int16_t)sig_ad[i][1]);
	}
	printf ("];\r\n");
	printf ("\r\ncur=[");
	for(i=0; i<DEFAULT_PERIOD; i++){
		printf("%d,",(int16_t)sig_ad[i][0]);
	}
	printf ("];\r\n");
}
void SIG_PrintfSinCos(void)
{
	uint32_t i;
	printf("\r\ncos[]={");
	for(i=0; i<DEFAULT_PERIOD; i++){
		printf("%f,",cos(2.0*PI*i/DEFAULT_PERIOD));
	}
	printf("];\r\n");
	printf("\r\nsin[]={");
	for(i=0; i<DEFAULT_PERIOD; i++){
		printf("%f,",sin(2.0*PI*i/DEFAULT_PERIOD));
	}
	printf("];\r\n");
}
uint8_t capn;
double captab[10];
void SIG_PrintfAD (void)
{
	double valuef = 0;
	double vol_real=0,vol_imag=0;
	double cur_real=0,cur_imag=0;
	double cur_ang,vol_ang,ang;
	double cur_amp,vol_amp;
	double vol,cur;
	double cap;
	double sysrr;
	uint16_t i;

	for(i=0; i<DEFAULT_PERIOD; i++){
		if((sig_ad[i][1]) == 0x7fff){
			printf("\r\nvolover");
		}
		if((sig_ad[i][1]) == 0x8000){
			printf("\r\nvolover");
		}
		if((sig_ad[i][0]) == 0x7fff){
			printf("\r\ncurover");
		}
		if((sig_ad[i][0]) == 0x8000){
			printf("\r\ncurover");
		}
	}
	for(i=0; i<DEFAULT_PERIOD; i++){
		valuef=(int16_t)sig_ad[i][1];
		vol_real += valuef*cos_tab[i];//cos(2.0*PI*i/DEFAULT_PERIOD);
		vol_imag += valuef*sin_tab[i];//sin(2.0*PI*i/DEFAULT_PERIOD);
	}
	for(i=0; i<DEFAULT_PERIOD; i++){
		valuef=(int16_t)sig_ad[i][0];
		cur_real += valuef*cos_tab[i];//cos(2.0*PI*i/DEFAULT_PERIOD);
		cur_imag += valuef*sin_tab[i];//sin(2.0*PI*i/DEFAULT_PERIOD);
	}
	vol_ang = atan2(vol_imag, vol_real);
	cur_ang = atan2(cur_imag, cur_real);
	vol_amp = sqrt(vol_real*vol_real + vol_imag*vol_imag);
	vol_amp /= DEFAULT_PERIOD;
	cur_amp = sqrt(cur_real*cur_real + cur_imag*cur_imag);
	cur_amp /= DEFAULT_PERIOD;
	//printf("\r\nvol_ang=%f;\r\ncur_ang=%f;",vol_ang,cur_ang);
	printf("\r\nvol_amp=%f;cur_amp=%f;v_c=%f;",vol_amp,cur_amp,vol_amp/cur_amp);
	ang = ((vol_ang-cur_ang)/PI)*180;
	if(ang < 0){
		ang+=360;
	}
	printf("ang=%f;",ang);	
	vol = vol_amp/32768.0;
	vol = vol*21.0;
	vol *= 2.5;
	vol *= 2;
	
	cur = cur_amp/1000.0;//amp
	cur /= 32768.0;
	cur *= 2.5;
	cur *= 2;
	sysrr = vol/cur;
	sysrr *= cos(vol_ang-cur_ang);
	//printf("\r\nsyscr=%f",(vol_amp/cur_amp)*sin(vol_ang-cur_ang));
	
	cap = vol/cur;
	cap *= sin(vol_ang-cur_ang);
	cap *= 2.0*PI*5.0;
	cap = 1/cap;	
	//cap = (1/(vol/cur)*sin(vol_ang-cur_ang)*2*PI*5);
	cap *= 1000000.0f;	
	printf("\r\nsysrr=%f;vol=%f;cur=%f;",sysrr,vol,cur*1000.0);
	printf("\r\ncap=%f;",cap);
	printf("cap21=%f;",cap*20);
	printf("cap201=%f;",cap*192);
	captab[capn] = cap*192;
	capn++;

	printf("\r\n");
}
void SIG_PrintfConverNum(uint8_t n)
{
	double c;
	
	capn = 0;	
	while(n--){
		SIG_StartConverCollect();
		while((SIGPRC_Struct.status&SIG_STAT_COLING)==SIG_STAT_COLING){
		}
		if(capn==10){
			c =captab[0];
			c+=captab[1];
			c+=captab[2];
			c+=captab[3];
			c+=captab[4];
			c+=captab[5];
			c+=captab[6];
			c+=captab[7];
			c+=captab[8];
			c+=captab[9];
			c/=10;
			printf("\r\n------------------------------------------------");
			printf("\r\ncap=%f",c);
			printf("\r\n------------------------------------------------");
			c = 0;
			while(c<100000){
				c+= 0.1;
			}
			capn = 0;
		}
		SIG_PrintfAD ();
	}
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_GetAD7687data (uint16_t *ad)
{
	AD7687_GetConverData (ad);
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StartConverCollect (void)
{
	SIGPRC_Struct.collect_counter = 0;//计数清0
	SIGPRC_Struct.status |= SIG_STAT_COLING;//正在采集
	SIGPRC_Struct.status &= ~(SIG_STAT_IDLE|
						SIG_STAT_COMPLETE);//清除空闲和完成标志	
	AD7687_StartConver ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StopConverCollect (void)
{
	AD7687_StopConver ();
	SIGPRC_Struct.status |= SIG_STAT_IDLE;
	SIGPRC_Struct.status &= ~SIG_STAT_COLING;
}
/**
  * @brief  设置完成标志，清楚采集标志，停止转换.
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_CompleteConverCollect (void)
{
	SIGPRC_Struct.status |= SIG_STAT_COMPLETE;
	SIG_StopConverCollect ();
}
/**
  * @brief  开始对地电阻采集.
  * 		设置对地电阻启动测试标志
  * 		先采集母线电压
  * 		正对地电压
  * 		负对地电压
  * 		计算
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StartConverEarthResCollect (void)
{
	RL_ZRelayConnect ();
	RL_FRelayConnect ();
	ADG_SetChannel (ADG_EARTHRES_CHANNEL);
	STK_delay10ms (1);
	RL_ZRelayHiZ ();
	RL_FRelayHiZ ();
	STK_delay10ms (10);//延时一会，使电压稳定
	/* 设置正在对地电阻采集标志 */
	SIGPRC_Struct.status |= SIG_STAT_ERCLG;
	/* 设置母线电压采集标志 */
	SIGPRC_Struct.status |= SIG_STAT_ERCLG_GE;
	SIG_StartConverCollect ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StopConverEarthResCollect (void)
{
	SIGPRC_Struct.status &= ~(SIG_STAT_ERCLG| SIG_STAT_ERCLG_GE|
							SIG_STAT_ERCLG_RZ| SIG_STAT_ERCLG_FZ);
	SIGPRC_Struct.status |= SIG_STAT_MEASUREOK;
	SIGPRC_Struct.status &= ~(SIG_STAT_CANCEL |
								SIG_STAT_MEASURING |
								SIG_STAT_READY);
	SIG_StopConverCollect ();
}
/**
  * @brief  对地电容采集
  * 		1.采集对地电阻
  * 		2.采集对地电容.
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StartConverEarthCapCollect (void)
{
	SIGPRC_Struct.status |= SIG_STAT_ECCLG_CP_FIRST;
	SIGPRC_Struct.status &= ~SIG_STAT_EARTHLINE;
	RL_ZRelayConnect ();
	RL_FRelayConnect ();
	WV_closeMl2035 ();
	ADG_SetChannel (ADG_EARTHRES_CHANNEL);
	STK_delay10ms (2);
	RL_ZRelayHiZ ();
	RL_FRelayHiZ ();
	STK_delay10ms (10);//延时一会，使电压稳定
	/* 设置正在对地电阻采集标志 */
	SIGPRC_Struct.status |= SIG_STAT_ECCLG;
	/* 设置母线电压采集标志 */
	SIGPRC_Struct.status |= SIG_STAT_ECCLG_GE;
	SIG_StartConverCollect ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StopConverEarthCapCollect (void)
{
	SIGPRC_Struct.status &= ~(SIG_STAT_ECCLG| SIG_STAT_ECCLG_GE|
							SIG_STAT_ECCLG_RZ| SIG_STAT_ECCLG_RF|
							SIG_STAT_ECCLG_CP);
	SIGPRC_Struct.status |= SIG_STAT_MEASUREOK;
	SIGPRC_Struct.status &= ~(SIG_STAT_CANCEL |
								SIG_STAT_MEASURING |
								SIG_STAT_READY);
	WV_closeMl2035 ();
	SIG_StopConverCollect ();
}
uint32_t signalamp = 0;
void _SIG_ControlSignalAmp (void)
{
	WV_SetAmplitude (signalamp);
}
void SIG_StartConverEarthSchCollect (void)
{
	uint32_t time;
	SIGPRC_Struct.status |= SIG_STAT_ECCLG_CP_FIRST;
	RL_ZRelayDisconnect ();
	RL_FRelayDisconnect ();
	ADG_SetChannel (ADG_CAP_CHANNEL3x);
	signalamp = 1000;
	_SIG_ControlSignalAmp ();
	WV_SetFrequency (5);
	//WV_SetAmplitude (100);
	STK_delay10ms (2);
	RL_ZRelayHiZ ();
	RL_FRelayHiZ ();
	for (time=0; time<col_delay; time++){
		STK_delay10ms (2);//延时一会，使电压稳定
		if(vk_msg == VK_BACK){
			break;
		}
	}
	SIGPRC_Struct.status |= SIG_STAT_ERSCH;
	SIGPRC_Struct.status |= SIG_STAT_ERSCH_DATA;
	SIGPRC_Struct.status &= ~(SIG_STAT_ERSCH_COM|SIG_STAT_ERSCH_SERCH|
						SIG_STAT_ERSCH_STOP| SIG_STAT_EARTHLINE);
	SIG_StartConverCollect ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StopConverEarthSchCollect (void)
{
	SIGPRC_Struct.status &= ~(SIG_STAT_ERSCH| SIG_STAT_ERSCH_COM| 
						SIG_STAT_ERSCH_SERCH| SIG_STAT_ERSCH_DATA|
						SIG_STAT_ERSCH_STOP| SIG_STAT_ERSCH_SERCHING);
	SIGPRC_Struct.status |= SIG_STAT_MEASUREOK;
	SIGPRC_Struct.status &= ~(SIG_STAT_CANCEL |
							SIG_STAT_MEASURING |
							SIG_STAT_READY);
	WV_closeMl2035 ();
	SIG_StopConverCollect ();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
extern volatile uint8_t vk_msg;
void SIG_EarthResCollectContinus (void)
{
	uint32_t time;
	if(SIGPRC_Struct.status & SIG_STAT_ERCLG_GE){
		RL_ZRelayConnect ();
		RL_FRelayDisconnect ();
		ADG_SetChannel (ADG_EARTHRES_CHANNEL);
		STK_delay10ms (2);
		RL_ZRelayHiZ ();
		RL_FRelayHiZ ();
		for (time=0; time<col_delay; time++){
			STK_delay10ms (1);//延时一会，使电压稳定
			if (vk_msg != VK_NOKEY){//如果有按键，退出
				break;
			}
		}
		SIGPRC_Struct.status &= ~SIG_STAT_ERCLG_GE;
		SIGPRC_Struct.status |= SIG_STAT_ERCLG_RZ;
		SIG_StartConverCollect ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ERCLG_RZ){
		RL_ZRelayDisconnect ();
		RL_FRelayConnect ();
		ADG_SetChannel (ADG_EARTHRES_CHANNEL);
		STK_delay10ms (2);
		RL_ZRelayHiZ ();
		RL_FRelayHiZ ();
		for (time=0; time<col_delay; time++){
			STK_delay10ms (1);//延时一会，使电压稳定
			if (vk_msg != VK_NOKEY){
				break;
			}
		}
		SIGPRC_Struct.status &= ~SIG_STAT_ERCLG_RZ;
		SIGPRC_Struct.status |= SIG_STAT_ERCLG_FZ;
		SIG_StartConverCollect ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ERCLG_FZ){
		SIG_StopConverEarthResCollect ();
	}
}
void SIG_EarthCapCollectContinus (void)
{
	uint32_t time;
	if(SIGPRC_Struct.status & SIG_STAT_ECCLG_GE){
		RL_ZRelayConnect ();
		RL_FRelayDisconnect ();
		ADG_SetChannel (ADG_EARTHRES_CHANNEL);
		STK_delay10ms (2);
		RL_ZRelayHiZ ();
		RL_FRelayHiZ ();
		for (time=0; time<col_delay; time++){
			STK_delay10ms (1);//延时一会，使电压稳定
			if (vk_msg != VK_NOKEY){//如果有按键，退出
				break;
			}
		}
		SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_GE;
		SIGPRC_Struct.status |= SIG_STAT_ECCLG_RZ;
		SIG_StartConverCollect ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG_RZ){
		RL_ZRelayDisconnect ();
		RL_FRelayConnect ();
		ADG_SetChannel (ADG_EARTHRES_CHANNEL);
		STK_delay10ms (1);
		RL_ZRelayHiZ ();
		RL_FRelayHiZ ();
		for (time=0; time<col_delay; time++){
			STK_delay10ms (1);//延时一会，使电压稳定
			if (vk_msg != VK_NOKEY){
				break;
			}
		}
		SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_RZ;
		SIGPRC_Struct.status |= SIG_STAT_ECCLG_RF;
		SIG_StartConverCollect ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG_RF){
		RL_ZRelayDisconnect ();
		RL_FRelayDisconnect ();
		ADG_SetChannel (ADG_CAP_CHANNEL3x);
		signalamp = 500;
		_SIG_ControlSignalAmp ();
		WV_SetFrequency (5);
		//WV_SetAmplitude (100);
		STK_delay10ms (1);
		RL_ZRelayHiZ ();
		RL_FRelayHiZ ();
		for (time=0; time<col_delay; time++){
			STK_delay10ms (1);//延时一会，使电压稳定
			if(vk_msg == VK_BACK){
				break;
			}
		}
		SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_RF;
		SIGPRC_Struct.status |= SIG_STAT_ECCLG_CP;
		SIGPRC_Struct.status |= SIG_STAT_ECCLG_CP_FIRST;
		SIG_StartConverCollect ();
	}else if(SIGPRC_Struct.status & SIG_STAT_ECCLG_CP){
		SIG_StopConverEarthCapCollect ();
	}
}
/**
  * @brief  开始采集母线电压
  * @param  None.
  * @retval None.
  * @verify .
  */
void SIG_StartConverGeneratrix (void)
{
	RL_ZRelayConnect ();
	RL_FRelayConnect ();
	ADG_SetChannel (ADG_EARTHRES_CHANNEL);
	STK_delay10ms (1);
	RL_ZRelayHiZ ();
	RL_FRelayHiZ ();
	SIG_StartConverCollect();
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */
/**
  * @brief  零偏校准函数
  * 		校准完毕后自动填入内存
  * 		需要校准四个通道.
  * @param  None.
  * @retval None.
  * @verify .
  */
/* 交流零偏校准，取得短路时的电容大小 */
void SIG_AcZeroDiff(void)
{
	double max_fft1,CurReal1,CurImag1,VolReal1;
	double VolImag1,cur1,vol1,cr1,crsum1,ang1;
	uint8_t status;
	uint16_t i;
	int16_t cap_tmp;
	WV_SetFrequency (5);
	signalamp = 1000;
	WV_SetAmplitude (signalamp);
	ADG_SetChannel(ADG_CAP_CHANNEL3x);
	STK_delay10ms (100);
	CurReal1 = 0;
	CurImag1 = 0;
	/* 先调节电流 尽量大 */
	while(1){
		SIG_StartConverCollect();
		while((SIGPRC_Struct.status&SIG_STAT_COLING)==SIG_STAT_COLING){
		}
		for(i=0; i<DEFAULT_PERIOD; i++){
			cap_tmp = (int16_t)sig_ad[i][0];
			CurReal1 += cap_tmp*cos_tab[i];
			CurImag1 += cap_tmp*sin_tab[i];
		}
		max_fft1 = 32768*DEFAULT_PERIOD;
		max_fft1 /= 2;
		cur1 = sqrt(CurReal1*CurReal1 + CurImag1*CurImag1);
		printf("\r\ncur1:%f,%f",cur1,max_fft1);
		signalamp *= (max_fft1/cur1);
		if(signalamp > 2500){
			signalamp = 2500;
		}
		printf("\r\nsignal:%u",signalamp);
		WV_SetAmplitude(signalamp);
		STK_delay10ms (50);
		break;
	}
	while(1){
		SIG_StartConverCollect();
		while((SIGPRC_Struct.status&SIG_STAT_COLING)==SIG_STAT_COLING){
		}
		for(i=0; i<DEFAULT_PERIOD; i++){
			if((sig_ad[i][0]) == 0x7fff){
				printf("\r\ncurover");
				signalamp -= 10;
				WV_SetAmplitude(signalamp);
				STK_delay10ms(50);
				break;
			}
			if((sig_ad[i][0]) == 0x8000){
				printf("\r\ncurover");
				signalamp -= 10;
				WV_SetAmplitude(signalamp);
				STK_delay10ms(50);
				break;
			}
		}
		if(i==DEFAULT_PERIOD){
			ADG_SetChannel(ADG_CAP_CHANNEL3x);
			break;
		}
	}
	STK_delay10ms (1);
	status = 0;
	crsum1 = 0;
	/* 求3x档位电容 */
	while(1){
		SIG_StartConverCollect();
		while((SIGPRC_Struct.status&SIG_STAT_COLING)==SIG_STAT_COLING){
		}
		CurReal1 = 0;
		CurImag1 = 0;
		VolReal1 = 0;
		VolImag1 = 0;
		for(i=0; i<DEFAULT_PERIOD; i++){
			cap_tmp = (int16_t)sig_ad[i][0];
			CurReal1 += cap_tmp*cos_tab[i];
			CurImag1 += cap_tmp*sin_tab[i];

			cap_tmp = (int16_t)sig_ad[i][1];
			VolReal1 += cap_tmp*cos_tab[i];
			VolImag1 += cap_tmp*sin_tab[i];
		}
		ang1 = atan2(VolImag1, VolReal1);
		ang1 -= atan2(CurImag1, CurReal1);

		cur1 = sqrt(CurReal1*CurReal1 + CurImag1*CurImag1);
		cur1 /= SIG_CUR_RESAMP;

		vol1 = sqrt(VolReal1*VolReal1 + VolImag1*VolImag1);
		vol1 *= SIG_VOL_RESAMP;

		cr1 = ((vol1/cur1)*sin(ang1));
		if(cr1<0){
			crsum1 -= cr1;
		}else{
			crsum1 += cr1;
		}
		status ++;
		if(status==20){
			crsum1 /= SIG_CAP3X_AMP;
			crsum1 /= 20;
			crsum1 *= (2.0*PI*5.0);
			crsum1 = 1/crsum1;
			crsum1 *= 1000000;
			SIGPRC_Struct.SIG_SERIES_CAP[2] = (int32_t)crsum1;
			printf("\r\n3x:%d",SIGPRC_Struct.SIG_SERIES_CAP[2]);
			ADG_SetChannel (ADG_CAP_CHANNEL2x);
			break;
		}
	}
	STK_delay10ms (1);
	status = 0;
	crsum1 = 0;
	/* 求2x档位电容 */
	while(1){
		SIG_StartConverCollect();
		while((SIGPRC_Struct.status&SIG_STAT_COLING)==SIG_STAT_COLING){
		}
		CurReal1 = 0;
		CurImag1 = 0;
		VolReal1 = 0;
		VolImag1 = 0;
		for(i=0; i<DEFAULT_PERIOD; i++){
			cap_tmp = (int16_t)sig_ad[i][0];
			CurReal1 += cap_tmp*cos_tab[i];
			CurImag1 += cap_tmp*sin_tab[i];

			cap_tmp = (int16_t)sig_ad[i][1];
			VolReal1 += cap_tmp*cos_tab[i];
			VolImag1 += cap_tmp*sin_tab[i];
		}
		ang1 = atan2(VolImag1, VolReal1);
		ang1 -= atan2(CurImag1, CurReal1);

		cur1 = sqrt(CurReal1*CurReal1 + CurImag1*CurImag1);
		cur1 /= SIG_CUR_RESAMP;

		vol1 = sqrt(VolReal1*VolReal1 + VolImag1*VolImag1);
		vol1 *= SIG_VOL_RESAMP;

		cr1 = ((vol1/cur1)*sin(ang1));
		if(cr1<0){
			crsum1 -= cr1;
		}else{
			crsum1 += cr1;
		}
		status ++;
		if(status==20){
			crsum1 /= SIG_CAP2X_AMP;
			crsum1 /= 20;
			crsum1 *= (2.0*PI*5.0);
			crsum1 = 1/crsum1;
			crsum1 *= 1000000;
			SIGPRC_Struct.SIG_SERIES_CAP[1] = (int32_t)crsum1;
			printf("\r\n2x:%d",SIGPRC_Struct.SIG_SERIES_CAP[1]);
			ADG_SetChannel (ADG_CAP_CHANNEL1x);
			break;
		}
	}
	crsum1 = 0;
	STK_delay10ms (1);
	status = 0;
	/* 求1x档位电容 */
	while(1){
		SIG_StartConverCollect();
		while((SIGPRC_Struct.status&SIG_STAT_COLING)==SIG_STAT_COLING){
		}
		CurReal1 = 0;
		CurImag1 = 0;
		VolReal1 = 0;
		VolImag1 = 0;
		for(i=0; i<DEFAULT_PERIOD; i++){
			cap_tmp = (int16_t)sig_ad[i][0];
			CurReal1 += cap_tmp*cos_tab[i];
			CurImag1 += cap_tmp*sin_tab[i];

			cap_tmp = (int16_t)sig_ad[i][1];
			VolReal1 += cap_tmp*cos_tab[i];
			VolImag1 += cap_tmp*sin_tab[i];
		}
		ang1 = atan2(VolImag1, VolReal1);
		ang1 -= atan2(CurImag1, CurReal1);

		cur1 = sqrt(CurReal1*CurReal1 + CurImag1*CurImag1);
		cur1 /= SIG_CUR_RESAMP;

		vol1 = sqrt(VolReal1*VolReal1 + VolImag1*VolImag1);
		vol1 *= SIG_VOL_RESAMP;

		cr1 = ((vol1/cur1)*sin(ang1));
		if(cr1<0){
			crsum1 -= cr1;
		}else{
			crsum1 += cr1;
		}
		status ++;
		if(status==20){
			crsum1 /= SIG_CAP1X_AMP;
			crsum1 /= 20;
			crsum1 *= (2.0*PI*5.0);
			crsum1 = 1/crsum1;
			crsum1 *= 1000000;
			SIGPRC_Struct.SIG_SERIES_CAP[0] = (int32_t)crsum1;
			printf("\r\n1x:%d",SIGPRC_Struct.SIG_SERIES_CAP[0]);
			break;
		}
	}
}
void SIG_ZeroDiff (void)
{
	double s_value[4];
	RL_ZRelayDisconnect ();
	RL_FRelayDisconnect ();
	STK_delay10ms (10);
	RL_ZRelayHiZ ();
	RL_FRelayHiZ ();
	/* 先对对地电阻两个通道进行零偏校准 */
	ADG_SetChannel (ADG_EARTHRES_CHANNEL);
	STK_delay10ms (100);
	SIG_StartConverCollect();
	while(SIGPRC_Struct.status & SIG_STAT_COLING);
	s_value[0] = SIG_GetZVOLvalues ();
	s_value[1] = SIG_GetFVOLvalues ();
	/* 对交流电流和电压两个通道进行零偏校准 */
	RL_ZRelayDisconnect ();
	RL_FRelayDisconnect ();
	STK_delay10ms (10);
	RL_ZRelayHiZ ();
	RL_FRelayHiZ ();
	STK_delay10ms (100);
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RZ] = (int32_t)s_value[0];
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_RF] = (int32_t)s_value[1];
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_CC] = 0;
	SIGPRC_Struct.SIG_ZERODRIFT[SIGCHN_VV] = 0;
	SIG_AcZeroDiff ();
	SIG_SaveSystemParam ();
	printf("\r\nz:%f\nf:%f\nc:%f\nv:%f\r\n",s_value[0],
			s_value[1],s_value[2],s_value[3]);
}
/**
  * @brief  U1/Rf+U1/R1 = U2/Rz   U1为负向导通测得电压
  * 		U3/Rz+U3/R1 = U4/Rf.  U3为正向导通测的电压
  * 							  R1为分压电阻
  * 		1/Rf = (U3*Umu/R1)/(U1U3-U2U4)
  * 		1/Rz = (U1*Umu/R1)/(U1U3-U2U4)
  * @param  ge:母线电压AD rz:正对地电压AD rf:负对地电压.
  * @retval None.
  * @verify .
  */
void SIG_CalcEarthRes (double uge, double urz, double urf)
{
	double Rz,Rf,R1;
	double U1U3_U2U4;
	if(uge<0){
		uge = -uge;
	}
	if(urz<0){
		urz = -urz;
	}
	if(urf<0){
		urf = -urf;
	}
	printf("\r\nuge:%f,urz:%f,urf:%f",uge,urz,urf);
	R1 = 201;//千欧
	U1U3_U2U4 = urz*urf-(uge-urz)*(uge-urf);
	Rf = urz*uge/R1;
	Rf = U1U3_U2U4 / Rf;
	Rz = urf*uge/R1;
	Rz = U1U3_U2U4 / Rz;
	if(Rz < 0){
		Rz = -Rz;
	}
	if(Rf < 0){
		Rf = -Rf;
	}
	printf("\r\nRz:%f,Rf:%f",Rz,Rf);
	if(Rz >= 100 || Rf >= 100){
		SIGPRC_Struct.status |= SIG_STAT_EARTHLINE;
	}else{
		SIGPRC_Struct.status &= ~SIG_STAT_EARTHLINE;
	}
	/* 乘上系数,与50K并联 */
	Rz = (Rz*50.0)/(Rz-50.0);
	Rf = (Rf*50.0)/(Rf-50.0);
	if(Rz<0){
		Rz = 0-Rz;
	}
	if(Rf<0){
		Rf = 0-Rf;
	}
	Rz = Rz*SIGPRC_Struct.SIG_ERRCOE_RZ/1000.0;
	Rf = Rf*SIGPRC_Struct.SIG_ERRCOE_RF/1000.0;
	SIGPRC_Struct.ZR_RES = Rz;
	SIGPRC_Struct.FR_RES = Rf;
	uge *= 2.5;
	uge /= 32768;
	uge /= 2;
	uge *= 201;
	SIGPRC_Struct.GE_VOL = uge;
}
/**
  * @brief  .
  * @param  None.
  * @retval None.
  * @verify .
  */

double CurReal;
double CurImag;
double VolReal;
double VolImag;
double crsum;//容抗和
double ressum;//阻抗和
void SIG_CalcEarthCap (void)
{
	static uint8_t statu;
	double ang,cur,vol,cr,res;
	double max_fft;//
	float div;
	int16_t cap_tmp;
	register uint32_t i=0;
	SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_CP_OK;
	if(SIGPRC_Struct.status & SIG_STAT_ECCLG_CP_FIRST){
		statu = 0;
		SIGPRC_Struct.status &= ~SIG_STAT_ECCLG_CP_FIRST;
	}
	if(statu == 0){//判断电流 使电流尽量大
		max_fft = 32768*DEFAULT_PERIOD;
		max_fft /= 2;
		CurReal = 0;
		CurImag = 0;
		for(i=0; i<DEFAULT_PERIOD; i++){
			cap_tmp = (int16_t)sig_ad[i][0];
			CurReal += cap_tmp*cos_tab[i];
			CurImag += cap_tmp*sin_tab[i];
		}
		cur = sqrt(CurReal*CurReal + CurImag*CurImag);
		div = max_fft / cur;
		printf("\r\ndiv:%f,max_fft:%f,cur_sum:%f",div,max_fft,cur);
		signalamp *= div;
		if(signalamp >2500){
			signalamp = 2500;
		}
		printf("\r\nsignalamp:%u",signalamp);
		WV_SetAmplitude (signalamp);
		STK_delay10ms (100);//等待信号稳定
		statu = 1;
	}else if(statu == 1){//判断电流是否超量程
		for(i=0; i<DEFAULT_PERIOD; i++){
			if((sig_ad[i][0]) == 0x7fff){
				printf("\r\ncurover");
				signalamp -= 20;
				WV_SetAmplitude(signalamp);
				STK_delay10ms(100);
				break;
			}
			if((sig_ad[i][0]) == 0x8000){
				printf("\r\ncurover");
				signalamp -= 20;
				WV_SetAmplitude(signalamp);
				STK_delay10ms(100);
				break;
			}
		}
		if(i==DEFAULT_PERIOD){
			for(i=0; i<DEFAULT_PERIOD; i++){
				if((sig_ad[i][1]) == 0x7fff){
					printf("\r\nvolover1");
					ADG_SetChannel(ADG_CAP_CHANNEL2x);
					break;
				}
				if((sig_ad[i][1]) == 0x8000){
					printf("\r\nvolover1");
					ADG_SetChannel(ADG_CAP_CHANNEL2x);
					break;
				}
			}
			statu = 2;
		}
	}else if(statu == 2){//判断电压是否超量程
		for(i=0; i<DEFAULT_PERIOD; i++){
			if((sig_ad[i][1]) == 0x7fff){
				printf("\r\nvolover");
				ADG_SetChannel(ADG_CAP_CHANNEL1x);
				break;
			}
			if((sig_ad[i][1]) == 0x8000){
				printf("\r\nvolover");
				ADG_SetChannel(ADG_CAP_CHANNEL1x);
				break;
			}
		}
		STK_delay10ms (10);
		crsum = 0;
		ressum = 0;
		statu = 3;
	}else if(statu < 23){
		CurReal = 0;
		CurImag = 0;
		VolReal = 0;
		VolImag = 0;
		for(i=0; i<DEFAULT_PERIOD; i++){
			cap_tmp = (int16_t)sig_ad[i][0];
			CurReal += cap_tmp*cos_tab[i];
			CurImag += cap_tmp*sin_tab[i];

			cap_tmp = (int16_t)sig_ad[i][1];
			VolReal += cap_tmp*cos_tab[i];
			VolImag += cap_tmp*sin_tab[i];
		}
		ang = atan2(VolImag, VolReal);
		ang -= atan2(CurImag, CurReal);

		cur = sqrt(CurReal*CurReal + CurImag*CurImag);
		cur /= SIG_CUR_RESAMP;

		vol = sqrt(VolReal*VolReal + VolImag*VolImag);
		vol *= SIG_VOL_RESAMP;

		cr = ((vol/cur)*sin(ang));
		res = ((vol/cur)*cos(ang));

		if(cr<0){
			crsum -= cr;
		}else{
			crsum += cr;
		}
		if(res<0){
			ressum -= res;
		}else{
			ressum += res;
		}
		statu++;
		//printf("\r\nstatu:%u,crsum:%f,ressum:%f",statu,crsum,ressum);
	}
	if(statu >= 23){//进行计算电容
		if(ADG_Struct.CHN == ADG_CAP_CHANNEL3x){
			crsum /= SIG_CAP3X_AMP;
			ressum /= SIG_CAP3X_AMP;
		}else if(ADG_Struct.CHN == ADG_CAP_CHANNEL2x){
			crsum /= SIG_CAP2X_AMP;
			ressum /= SIG_CAP2X_AMP;
		}else if(ADG_Struct.CHN == ADG_CAP_CHANNEL1x){
			crsum /= SIG_CAP1X_AMP;
			ressum /= SIG_CAP1X_AMP;
		}
		//ressum *= (SIGPRC_Struct.SIG_ERRCOE_RR/1000.0);
		crsum /= 20.0;
		ressum /= 20.0;
		SIGPRC_Struct.CR_SYS = crsum/1000.0;
		SIGPRC_Struct.RR_SYS = ressum/1000.0;
		/* 计算标准电容和测量电容串联 */
		SIGPRC_Struct.CP_CAP = crsum;
		SIGPRC_Struct.CP_CAP *= 2.0*PI*5.0;
		SIGPRC_Struct.CP_CAP = 1/SIGPRC_Struct.CP_CAP;
		SIGPRC_Struct.CP_CAP *= 1000000;//放大到uf单位
		/* 求得测量电容，采用并联法反向求得 */
		if(ADG_Struct.CHN == ADG_CAP_CHANNEL3x){
			ressum = SIGPRC_Struct.SIG_SERIES_CAP[2];
			printf("\r\ncap2:%u",SIGPRC_Struct.SIG_SERIES_CAP[2]);
		}else if(ADG_Struct.CHN == ADG_CAP_CHANNEL2x){
			ressum = SIGPRC_Struct.SIG_SERIES_CAP[1];
		}else if(ADG_Struct.CHN == ADG_CAP_CHANNEL1x){
			ressum = SIGPRC_Struct.SIG_SERIES_CAP[0];
		}
		printf("\r\ncap2:%u,%u,%u",SIGPRC_Struct.SIG_SERIES_CAP[2],
				SIGPRC_Struct.SIG_SERIES_CAP[1],SIGPRC_Struct.SIG_SERIES_CAP[0]);
		printf("\r\nseriescap:%f",ressum);
		crsum = ressum - SIGPRC_Struct.CP_CAP;
		printf("\r\ndel:%f",crsum);
		if(crsum == 0){
			crsum = 0.1;
		}
		SIGPRC_Struct.CP_CAP = (ressum*SIGPRC_Struct.CP_CAP)/crsum;
		/* 乘以相应档位的误差系数*/
		if(ADG_Struct.CHN == ADG_CAP_CHANNEL3x){
			SIGPRC_Struct.CP_CAP *= (SIGPRC_Struct.SIG_ERRCOE_CAP[2]/1000.0);
		}else if(ADG_Struct.CHN == ADG_CAP_CHANNEL2x){
			SIGPRC_Struct.CP_CAP *= (SIGPRC_Struct.SIG_ERRCOE_CAP[1]/1000.0);
		}else if(ADG_Struct.CHN == ADG_CAP_CHANNEL1x){
			SIGPRC_Struct.CP_CAP *= (SIGPRC_Struct.SIG_ERRCOE_CAP[0]/1000.0);
		}
		printf("\r\neecoe:%u,%u,%u",SIGPRC_Struct.SIG_ERRCOE_CAP[2],
			SIGPRC_Struct.SIG_ERRCOE_CAP[1],SIGPRC_Struct.SIG_ERRCOE_CAP[0]);
		SIGPRC_Struct.CR_SYS = 2*PI*5.0;
		SIGPRC_Struct.CR_SYS *= SIGPRC_Struct.CP_CAP;
		SIGPRC_Struct.CR_SYS /= 1000000;
		SIGPRC_Struct.CR_SYS = 1/SIGPRC_Struct.CR_SYS;
		SIGPRC_Struct.CR_SYS /= 1000.0;
		if(SIGPRC_Struct.RR_SYS >= 100){
			SIGPRC_Struct.status |= SIG_STAT_EARTHLINE;
		}else{
			SIGPRC_Struct.status &= ~SIG_STAT_EARTHLINE;
		}
		printf("\r\n容抗:%f 阻抗:%f 电容:%f",SIGPRC_Struct.CR_SYS,
						SIGPRC_Struct.RR_SYS,SIGPRC_Struct.CP_CAP);
		statu = 0;
		/* 黄色地线？ */
		SIGPRC_Struct.status |= SIG_STAT_ECCLG_CP_OK;
	}
}
/**
  * @brief      U1/Rf+U1/R1 = U2/Rz+U2/R2
  *             U3/Rz+U3/R1 = U4/Rf+U4/R2
  *这是一个关于 1/Rf和1/Rz的2元一次方程组  
  *消元法 解得  1/Rf = [U2(U3+U4)/R2-U3(U1+U2)/R1]/(U1U3-U2U4) 
  *             1/Rz = [U4(U1+U2)/R2-U1(U3+U4)/R1]/(U1U3-U2U4).
  * @param  None
  * @retval None
  */
void _SIG_Delay1ms (uint32_t delay)
{
	uint32_t MS;
	while (delay--){
		MS = 25000;
		while (MS--);
	}
}
/*********************************END OF FILE**********************************/


