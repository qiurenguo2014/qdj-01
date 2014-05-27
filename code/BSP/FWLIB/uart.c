/**
  ******************************************************************************
  * @file    uart.c
  * @author  YANDLD
  * @version V2.4
  * @date    2013.5.25
  * @brief   超核K60固件库 UART 串口 驱动库
  ******************************************************************************
  */
#include "uart.h"
#include "string.h"
typedef enum {FALSE = 0, TRUE = !FALSE} ErrorState;
//发送结构
UART_TxSendTypeDef UART_TxIntStruct1;
UART_Type* UART_DebugPort = NULL;

void UART_DebugPortInit(uint32_t UARTxMAP, uint32_t UART_BaudRate)
{
	UART_InitTypeDef UART_DebugInitStruct1;
	UART_MapTypeDef *pUART_Map = (UART_MapTypeDef*) & (UARTxMAP);
	//配置默认的调试UART串口
	UART_DebugInitStruct1.UART_BaudRate = UART_BaudRate;
	UART_DebugInitStruct1.UARTxMAP = UARTxMAP;

	//找出对应的UART端口
	switch(pUART_Map->UART_Index) {
	case 0:
		UART_DebugPort = UART0;
		break;

	case 1:
		UART_DebugPort = UART1;
		break;

	case 2:
		UART_DebugPort = UART2;
		break;

	case 3:
		UART_DebugPort = UART3;
		break;

	case 4:
		UART_DebugPort = UART4;
		break;

	default:
		UART_DebugPort = NULL;
		break;
	}

	UART_Init(&UART_DebugInitStruct1);
}
/***********************************************************************************************
 功能：初始化串口
 形参：UART_InitStruct UART初始化结构
 返回：0
 详解：0
************************************************************************************************/
void UART_Init(UART_InitTypeDef* UART_InitStruct)
{
	UART_Type* UARTx = NULL;
	PORT_Type *UART_PORT = NULL;
	uint16_t sbr;
	uint8_t brfa;
	uint32_t clock;
	UART_MapTypeDef *pUART_Map = NULL;
	pUART_Map = (UART_MapTypeDef*) & (UART_InitStruct->UARTxMAP);
	//检测参数
	assert_param(IS_UART_MAP(UART_InitStruct->UARTxMAP));

	//找出对应的UART端口
	switch(pUART_Map->UART_Index) {
	case 0:
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
		UARTx = UART0;
		break;

	case 1:
		SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
		UARTx = UART1;
		break;

	case 2:
		SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
		UARTx = UART2;
		break;

	case 3:
		SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
		UARTx = UART3;
		break;

	case 4:
		SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
		UARTx = UART4;
		break;

	default:
		UARTx = NULL;
		break;
	}

	//找出 PORT端口 并使能时钟
	switch(pUART_Map->UART_GPIO_Index) {
	case 0:
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
		UART_PORT = PORTA;
		break;

	case 1:
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
		UART_PORT = PORTB;
		break;

	case 2:
		SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
		UART_PORT = PORTC;
		break;

	case 3:
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
		UART_PORT = PORTD;
		break;

	case 4:
		SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
		UART_PORT = PORTE;
		break;

	default:
		break;
	}

	//配置对应引脚为串口模式
	UART_PORT->PCR[pUART_Map->UART_RX_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[pUART_Map->UART_RX_Pin_Index] |= PORT_PCR_MUX(pUART_Map->UART_Alt_Index);
	UART_PORT->PCR[pUART_Map->UART_TX_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[pUART_Map->UART_TX_Pin_Index] |= PORT_PCR_MUX(pUART_Map->UART_Alt_Index);
	//配置传输频率
	GetCPUInfo();  //计算系统时钟
	clock = CPUInfo.BusClock;

	if((uint32_t)UARTx == UART0_BASE || (uint32_t)UARTx == UART1_BASE) {
		clock = CPUInfo.CoreClock; //UART0 UART1使用CoreClock
	}

	sbr = (uint16_t)((clock) / ((UART_InitStruct->UART_BaudRate) * 16));
	brfa = ((clock * 2) / (UART_InitStruct->UART_BaudRate) - (sbr * 32));
	UARTx->BDH |= ((sbr >> 8)&UART_BDH_SBR_MASK); //设置高5位的数据
	UARTx->BDL = (sbr & UART_BDL_SBR_MASK); //设置低8位数据
	UARTx->C4 |= brfa & (UART_BDL_SBR_MASK >> 3); //设置小数位
	//配置uart控制寄存器，实现基本的八位传输功能
	UARTx->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);	 //禁止发送接受
	UARTx->C1 &= ~UART_C1_M_MASK;                      //配置数据位数为8位
	UARTx->C1 &= ~(UART_C1_PE_MASK);                   //配置为无奇偶校检位
	UARTx->S2 &= ~UART_S2_MSBF_MASK;                   //配置为最低位优先传输
	//使能接收器与发送器
	UARTx->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);	 //开启数据发送接受,参见手册1221页
	//记录最大缓冲区数
	UART_TxIntStruct1.MaxBufferSize = MAX_TX_BUF_SIZE;
}

/***********************************************************************************************
 功能：配置中断开启
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4

			 UART_IT : 支持的中断
 返回：0
 详解：0
************************************************************************************************/
void UART_ITConfig(UART_Type* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
	//参数检查
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_IT(UART_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	switch(UART_IT) {
	case UART_IT_TDRE:
		(ENABLE == NewState) ? (UARTx->C2 |= UART_C2_TIE_MASK) : (UARTx->C2 &= ~UART_C2_TIE_MASK);
		break;

	case UART_IT_TC:
		(ENABLE == NewState) ? (UARTx->C2 |= UART_C2_TCIE_MASK) : (UARTx->C2 &= ~UART_C2_TCIE_MASK);
		break;

	case UART_IT_RDRF:
		(ENABLE == NewState) ? (UARTx->C2 |= UART_C2_RIE_MASK) : (UARTx->C2 &= ~UART_C2_RIE_MASK);
		break;

	case UART_IT_IDLE:
		(ENABLE == NewState) ? (UARTx->C2 |= UART_C2_ILIE_MASK) : (UARTx->C2 &= ~UART_C2_ILIE_MASK);
		break;

	default:
		break;
	}
}
/***********************************************************************************************
 功能：获得中断标志
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4

			 UART_IT : 支持的中断
 返回：0
 详解：0
************************************************************************************************/
ITStatus UART_GetITStatus(UART_Type* UARTx, uint16_t UART_IT)
{
	ITStatus retval = RESET;
	//参数检查
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_UART_IT(UART_IT));

	switch(UART_IT) {
	case UART_IT_TDRE:
		(UARTx->S1 & UART_S1_TDRE_MASK) ? (retval = SET) : (retval = RESET);
		break;

	case UART_IT_TC:
		(UARTx->S1 & UART_S1_TC_MASK) ? (retval = SET) : (retval = RESET);
		break;

	case UART_IT_RDRF:
		(UARTx->S1 & UART_S1_RDRF_MASK) ? (retval = SET) : (retval = RESET);
		break;

	case UART_IT_IDLE:
		(UARTx->S1 & UART_S1_IDLE_MASK) ? (retval = SET) : (retval = RESET);
		break;

	default:
		break;
	}

	return retval;
}


/***********************************************************************************************
 功能：串口发送一个字节
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4

			 Data : 0-0xFF 发送的数据
 返回：0
 详解：0
************************************************************************************************/
void UART_SendData(UART_Type* UARTx, uint8_t Data)
{
	while(!(UARTx->S1 & UART_S1_TDRE_MASK));

	UARTx->D = (uint8_t)Data;
}
/***********************************************************************************************
 功能：使用中断发送串口数据
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4

			 *DataBuf : 发送的数据 缓冲区指针
			  Len     : 发送的数据长度
 返回：0
 详解：0
************************************************************************************************/
void UART_SendDataInt(UART_Type* UARTx, uint8_t* pBuffer, uint8_t NumberOfBytes)
{
	//参数检测
	assert_param(IS_UART_ALL_PERIPH(UARTx));

	//内存拷贝
	memcpy(UART_TxIntStruct1.TxBuf, pBuffer, NumberOfBytes);
	UART_TxIntStruct1.Length = NumberOfBytes;
	UART_TxIntStruct1.Offset = 0;
	UART_TxIntStruct1.IsComplete = FALSE;
	//使用中断方式传输 不使用DMA
	UARTx->C5 &= ~UART_C5_TDMAS_MASK;
	//使能传送中断
	UARTx->C2 |= UART_C2_TIE_MASK;
}
/***********************************************************************************************
 功能：开启UART DMA支持s
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4

			 UART_DMAReq : DMA中断源

			 NewState    : 使能或者关闭
			 @arg  ENABLE : 使能
			 @arg  DISABLE: 禁止
 返回：0
 详解：需要DMA构件的支持 需要使用DMA构件中的 Iscomplete函数判断是否发送完成
************************************************************************************************/
void UART_DMACmd(UART_Type* UARTx, uint16_t UART_DMAReq, FunctionalState NewState)
{
	//参数检查
	assert_param(IS_UART_IT(UART_DMAReq));
	assert_param(IS_UART_ALL_PERIPH(UARTx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	switch(UART_DMAReq) {
	case UART_DMAReq_Tx:
		(NewState == ENABLE) ? (UARTx->C5 |= UART_C5_TDMAS_MASK) : (UARTx->C5 &= ~UART_C5_TDMAS_MASK);
		break;

	case UART_DMAReq_Rx:
		(NewState == ENABLE) ? (UARTx->C5 |= UART_C5_RDMAS_MASK) : (UARTx->C5 &= ~UART_C5_RDMAS_MASK);
		break;

	default:
		break;
	}
}
/***********************************************************************************************
 功能：使用中断方式 发送串口数据 中断过程
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4
 返回：0
 详解：当开启串口中断发送时 在对应的串口中断中调用此过程
************************************************************************************************/
void UART_SendDataIntProcess(UART_Type* UARTx)
{
	if((UARTx->S1 & UART_S1_TDRE_MASK) && (UARTx->C2 & UART_C2_TIE_MASK)) {
		if(UART_TxIntStruct1.IsComplete == FALSE) {
			UARTx->D = UART_TxIntStruct1.TxBuf[UART_TxIntStruct1.Offset++];

			if(UART_TxIntStruct1.Offset >= UART_TxIntStruct1.Length) {
				UART_TxIntStruct1.IsComplete = TRUE;
				//关闭发送中断
				UARTx->C2 &= ~UART_C2_TIE_MASK;
			}
		}
	}
}
/***********************************************************************************************
 功能：串口接收一个字节
 形参：UART_Type 串口选择
			 @arg  UART0: 串口0
			 @arg  UART1: 串口1
			 @arg  UART2: 串口2
			 @arg  UART3: 串口3
			 @arg  UART4: 串口4

			 *ch : 接收到的字节 传递指针
 返回：0 接收失败
       1 接收成功
 详解：0
************************************************************************************************/
uint8_t UART_ReceiveData(UART_Type *UARTx, uint8_t *ch)
{
	if((UARTx->S1 & UART_S1_RDRF_MASK) != 0) { //判断接收缓冲区是否满
		*ch = (UARTx->D);	//接受数据
		return 1; 		  	//接受成功
	}

	return 0;			      //如果超时，接受失败
}
//内部函数为实现UART_printf
static void UART_puts(char *pch)
{
	while(*pch != '\0') {
		UART_SendData(UART_DebugPort, *pch);
		pch++;
	}
}
//内部函数为实现UART_printf
static void printn(unsigned int n, unsigned int b)
{
	const char *ntab = "0123456789ABCDEF";
	unsigned int a, m;

	if(n / b) {
		a = n / b;
		printn(a, b);
	}

	m = n % b;
	UART_SendData(UART_DebugPort, ntab[m]);
}
/***********************************************************************************************
 功能：UART 格式化输出
 形参：fmt 输入字符串指针
 返回：0
 详解：类似于C标准库中的printf 但是只支持 %d %l %o %x %s
************************************************************************************************/
void UART_printf(const char *fmt, ...)
{
	char c;
	unsigned int *adx = (unsigned int*)(void*)&fmt + 1;
_loop:

	while((c = *fmt++) != '%') {
		if(c == '\0') return;

		UART_SendData(UART_DebugPort, c);
	}

	c = *fmt++;

	if(c == 'd' || c == 'l') {
		printn(*adx, 10);
	}

	if(c == 'o' || c == 'x') {
		printn(*adx, c == 'o' ? 8 : 16);
	}

	if(c == 's') {
		UART_puts((char*)*adx);
	}

	adx++;
	goto _loop;
}
/***********************************************************************************************
 功能：打印处理器信息
 形参：0
 返回：0
 详解：针对于Freescale Kinetis系列
************************************************************************************************/
void DisplayCPUInfo(void)
{
	//打印固件库版本
	UART_printf("PH2803_CTSMP FW Version:%d\r\n", GetFWVersion());

	//打印标题
	UART_printf("====== System Information =======\r\n");

	//打印Kinetis系列型号
	if(CPUInfo.FamilyType != 0) {
		UART_printf("Family: \tK%d\r\n", CPUInfo.FamilyType);
	} else {
		UART_printf("\nUnrecognized Kinetis family device.\n");
	}

	//打印封装信息
	UART_printf("PinCnt: \t%d\r\n", CPUInfo.PinCnt);
	//打印PFlash大小
	UART_printf("PFlash Size: \t%d KB\r\n", CPUInfo.PFlashSize);
	//打印FlexNVM大小
	UART_printf("FlexNVM Size: \t%d KB\r\n", CPUInfo.FlexNVMSize);
	//打印RAM 大小
	UART_printf("RAM Size :\t%d KB\r\n", CPUInfo.RAMSize);
	//打印CoreClock
	UART_printf("CoreClock: \t%d MHz\r\n", CPUInfo.CoreClock / 1000000);
	//打印BusClock
	UART_printf("BusClock: \t%d MHz\r\n", CPUInfo.BusClock / 1000000);
	//打印FlexBusClock
	UART_printf("FlexBusClock: \t%d MHz\r\n", CPUInfo.FlexBusClock / 1000000);
	//打印FlashClock
	UART_printf("FlashClock: \t%d MHz\r\n", CPUInfo.FlashClock / 1000000);

	//打印结尾
	UART_printf("=================================\r\n");
}

