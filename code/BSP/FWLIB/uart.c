/**
  ******************************************************************************
  * @file    uart.c
  * @author  YANDLD
  * @version V2.4
  * @date    2013.5.25
  * @brief   ����K60�̼��� UART ���� ������
  ******************************************************************************
  */
#include "uart.h"
#include "string.h"
typedef enum {FALSE = 0, TRUE = !FALSE} ErrorState;
//���ͽṹ
UART_TxSendTypeDef UART_TxIntStruct1;
UART_Type* UART_DebugPort = NULL;

void UART_DebugPortInit(uint32_t UARTxMAP, uint32_t UART_BaudRate)
{
	UART_InitTypeDef UART_DebugInitStruct1;
	UART_MapTypeDef *pUART_Map = (UART_MapTypeDef*) & (UARTxMAP);
	//����Ĭ�ϵĵ���UART����
	UART_DebugInitStruct1.UART_BaudRate = UART_BaudRate;
	UART_DebugInitStruct1.UARTxMAP = UARTxMAP;

	//�ҳ���Ӧ��UART�˿�
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
 ���ܣ���ʼ������
 �βΣ�UART_InitStruct UART��ʼ���ṹ
 ���أ�0
 ��⣺0
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
	//������
	assert_param(IS_UART_MAP(UART_InitStruct->UARTxMAP));

	//�ҳ���Ӧ��UART�˿�
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

	//�ҳ� PORT�˿� ��ʹ��ʱ��
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

	//���ö�Ӧ����Ϊ����ģʽ
	UART_PORT->PCR[pUART_Map->UART_RX_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[pUART_Map->UART_RX_Pin_Index] |= PORT_PCR_MUX(pUART_Map->UART_Alt_Index);
	UART_PORT->PCR[pUART_Map->UART_TX_Pin_Index] &= ~PORT_PCR_MUX_MASK;
	UART_PORT->PCR[pUART_Map->UART_TX_Pin_Index] |= PORT_PCR_MUX(pUART_Map->UART_Alt_Index);
	//���ô���Ƶ��
	GetCPUInfo();  //����ϵͳʱ��
	clock = CPUInfo.BusClock;

	if((uint32_t)UARTx == UART0_BASE || (uint32_t)UARTx == UART1_BASE) {
		clock = CPUInfo.CoreClock; //UART0 UART1ʹ��CoreClock
	}

	sbr = (uint16_t)((clock) / ((UART_InitStruct->UART_BaudRate) * 16));
	brfa = ((clock * 2) / (UART_InitStruct->UART_BaudRate) - (sbr * 32));
	UARTx->BDH |= ((sbr >> 8)&UART_BDH_SBR_MASK); //���ø�5λ������
	UARTx->BDL = (sbr & UART_BDL_SBR_MASK); //���õ�8λ����
	UARTx->C4 |= brfa & (UART_BDL_SBR_MASK >> 3); //����С��λ
	//����uart���ƼĴ�����ʵ�ֻ����İ�λ���书��
	UARTx->C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);	 //��ֹ���ͽ���
	UARTx->C1 &= ~UART_C1_M_MASK;                      //��������λ��Ϊ8λ
	UARTx->C1 &= ~(UART_C1_PE_MASK);                   //����Ϊ����żУ��λ
	UARTx->S2 &= ~UART_S2_MSBF_MASK;                   //����Ϊ���λ���ȴ���
	//ʹ�ܽ������뷢����
	UARTx->C2 |= (UART_C2_RE_MASK | UART_C2_TE_MASK);	 //�������ݷ��ͽ���,�μ��ֲ�1221ҳ
	//��¼��󻺳�����
	UART_TxIntStruct1.MaxBufferSize = MAX_TX_BUF_SIZE;
}

/***********************************************************************************************
 ���ܣ������жϿ���
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4

			 UART_IT : ֧�ֵ��ж�
 ���أ�0
 ��⣺0
************************************************************************************************/
void UART_ITConfig(UART_Type* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
	//�������
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
 ���ܣ�����жϱ�־
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4

			 UART_IT : ֧�ֵ��ж�
 ���أ�0
 ��⣺0
************************************************************************************************/
ITStatus UART_GetITStatus(UART_Type* UARTx, uint16_t UART_IT)
{
	ITStatus retval = RESET;
	//�������
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
 ���ܣ����ڷ���һ���ֽ�
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4

			 Data : 0-0xFF ���͵�����
 ���أ�0
 ��⣺0
************************************************************************************************/
void UART_SendData(UART_Type* UARTx, uint8_t Data)
{
	while(!(UARTx->S1 & UART_S1_TDRE_MASK));

	UARTx->D = (uint8_t)Data;
}
/***********************************************************************************************
 ���ܣ�ʹ���жϷ��ʹ�������
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4

			 *DataBuf : ���͵����� ������ָ��
			  Len     : ���͵����ݳ���
 ���أ�0
 ��⣺0
************************************************************************************************/
void UART_SendDataInt(UART_Type* UARTx, uint8_t* pBuffer, uint8_t NumberOfBytes)
{
	//�������
	assert_param(IS_UART_ALL_PERIPH(UARTx));

	//�ڴ濽��
	memcpy(UART_TxIntStruct1.TxBuf, pBuffer, NumberOfBytes);
	UART_TxIntStruct1.Length = NumberOfBytes;
	UART_TxIntStruct1.Offset = 0;
	UART_TxIntStruct1.IsComplete = FALSE;
	//ʹ���жϷ�ʽ���� ��ʹ��DMA
	UARTx->C5 &= ~UART_C5_TDMAS_MASK;
	//ʹ�ܴ����ж�
	UARTx->C2 |= UART_C2_TIE_MASK;
}
/***********************************************************************************************
 ���ܣ�����UART DMA֧��s
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4

			 UART_DMAReq : DMA�ж�Դ

			 NewState    : ʹ�ܻ��߹ر�
			 @arg  ENABLE : ʹ��
			 @arg  DISABLE: ��ֹ
 ���أ�0
 ��⣺��ҪDMA������֧�� ��Ҫʹ��DMA�����е� Iscomplete�����ж��Ƿ������
************************************************************************************************/
void UART_DMACmd(UART_Type* UARTx, uint16_t UART_DMAReq, FunctionalState NewState)
{
	//�������
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
 ���ܣ�ʹ���жϷ�ʽ ���ʹ������� �жϹ���
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4
 ���أ�0
 ��⣺�����������жϷ���ʱ �ڶ�Ӧ�Ĵ����ж��е��ô˹���
************************************************************************************************/
void UART_SendDataIntProcess(UART_Type* UARTx)
{
	if((UARTx->S1 & UART_S1_TDRE_MASK) && (UARTx->C2 & UART_C2_TIE_MASK)) {
		if(UART_TxIntStruct1.IsComplete == FALSE) {
			UARTx->D = UART_TxIntStruct1.TxBuf[UART_TxIntStruct1.Offset++];

			if(UART_TxIntStruct1.Offset >= UART_TxIntStruct1.Length) {
				UART_TxIntStruct1.IsComplete = TRUE;
				//�رշ����ж�
				UARTx->C2 &= ~UART_C2_TIE_MASK;
			}
		}
	}
}
/***********************************************************************************************
 ���ܣ����ڽ���һ���ֽ�
 �βΣ�UART_Type ����ѡ��
			 @arg  UART0: ����0
			 @arg  UART1: ����1
			 @arg  UART2: ����2
			 @arg  UART3: ����3
			 @arg  UART4: ����4

			 *ch : ���յ����ֽ� ����ָ��
 ���أ�0 ����ʧ��
       1 ���ճɹ�
 ��⣺0
************************************************************************************************/
uint8_t UART_ReceiveData(UART_Type *UARTx, uint8_t *ch)
{
	if((UARTx->S1 & UART_S1_RDRF_MASK) != 0) { //�жϽ��ջ������Ƿ���
		*ch = (UARTx->D);	//��������
		return 1; 		  	//���ܳɹ�
	}

	return 0;			      //�����ʱ������ʧ��
}
//�ڲ�����Ϊʵ��UART_printf
static void UART_puts(char *pch)
{
	while(*pch != '\0') {
		UART_SendData(UART_DebugPort, *pch);
		pch++;
	}
}
//�ڲ�����Ϊʵ��UART_printf
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
 ���ܣ�UART ��ʽ�����
 �βΣ�fmt �����ַ���ָ��
 ���أ�0
 ��⣺������C��׼���е�printf ����ֻ֧�� %d %l %o %x %s
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
 ���ܣ���ӡ��������Ϣ
 �βΣ�0
 ���أ�0
 ��⣺�����Freescale Kinetisϵ��
************************************************************************************************/
void DisplayCPUInfo(void)
{
	//��ӡ�̼���汾
	UART_printf("PH2803_CTSMP FW Version:%d\r\n", GetFWVersion());

	//��ӡ����
	UART_printf("====== System Information =======\r\n");

	//��ӡKinetisϵ���ͺ�
	if(CPUInfo.FamilyType != 0) {
		UART_printf("Family: \tK%d\r\n", CPUInfo.FamilyType);
	} else {
		UART_printf("\nUnrecognized Kinetis family device.\n");
	}

	//��ӡ��װ��Ϣ
	UART_printf("PinCnt: \t%d\r\n", CPUInfo.PinCnt);
	//��ӡPFlash��С
	UART_printf("PFlash Size: \t%d KB\r\n", CPUInfo.PFlashSize);
	//��ӡFlexNVM��С
	UART_printf("FlexNVM Size: \t%d KB\r\n", CPUInfo.FlexNVMSize);
	//��ӡRAM ��С
	UART_printf("RAM Size :\t%d KB\r\n", CPUInfo.RAMSize);
	//��ӡCoreClock
	UART_printf("CoreClock: \t%d MHz\r\n", CPUInfo.CoreClock / 1000000);
	//��ӡBusClock
	UART_printf("BusClock: \t%d MHz\r\n", CPUInfo.BusClock / 1000000);
	//��ӡFlexBusClock
	UART_printf("FlexBusClock: \t%d MHz\r\n", CPUInfo.FlexBusClock / 1000000);
	//��ӡFlashClock
	UART_printf("FlashClock: \t%d MHz\r\n", CPUInfo.FlashClock / 1000000);

	//��ӡ��β
	UART_printf("=================================\r\n");
}

