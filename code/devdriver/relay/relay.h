/*========================================================================
#   FileName: relay.h
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-21 16:31:42
========================================================================*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RELAY_H
#define __RELAY_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#
typedef struct{
	uint32_t status;
	volatile long jif_value;		//在这个时间后可以恢复高阻
}RELAY_StructType;
extern RELAY_StructType relay;
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define Z_RL1 PTB
#define Z_RL2 PTB
#define F_RL1 PTB
#define F_RL2 PTB
#define Z_RL1_Pin GPIO_Pin_22
#define Z_RL2_Pin GPIO_Pin_23
#define F_RL1_Pin GPIO_Pin_20
#define F_RL2_Pin GPIO_Pin_21
#define RELAY_STTS_UPDATA (1<<0)
#define RELAY_Hiz_jif 2
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
extern void RL_Configuration (void);
extern void RL_ZRelayHiZ (void);
extern void RL_FRelayHiZ (void);
extern void RL_ZRelayConnect (void);
extern void RL_ZRelayDisconnect (void);
extern void RL_FRelayConnect (void);
extern void RL_FRelayDisconnect (void);
extern void RL_RelayTask (void);
#endif 

/*********************************END OF FILE**********************************/

