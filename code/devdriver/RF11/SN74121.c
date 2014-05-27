/**********************************************************************
* @File    : 	SN74121.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.10
* @Brief   : 	����̬����������
***********************************************************************/
#include "SN74121.h"


//* Private ---------------------------------------------------------*//

//================================
// ��ʼ������
//================================
static void sGPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// ���� SN74121_EN Ϊ�����
	GPIO_InitStruct.GPIOx = SN74121_ENA_PORT;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStruct.GPIO_Pin = SN74121_ENA_PIN;
	GPIO_Init(&GPIO_InitStruct);
	// ���� SN74121_EXT Ϊ�������룬�½����ж�
	GPIO_InitStruct.GPIOx = SN74121_EXT_PORT;
	GPIO_InitStruct.GPIO_InitState = Bit_SET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_FALLING;
	GPIO_InitStruct.GPIO_Pin = SN74121_EXT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(&GPIO_InitStruct);
}


//* Public ---------------------------------------------------------*//

//================================
// ��ʼ������̬������
//================================
void SN74121_Init()
{
#ifdef _SET_DEBUG_MODE_
	UART_printf("Init SN74121 Moniflop...\t\t\t");
#endif

	//��ʼ���˿�
	sGPIO_Configuration();

	NVIC_EnableIRQ(SN74121_EXT_IRQ);

#ifdef _SET_DEBUG_MODE_
	UART_printf("[OK]\r\n");
#endif
}

//================================
// ����������
//================================
void SN74121_SetEnabled()
{
#ifdef _SET_DEBUG_MODE_
	UART_printf("  Set SN74121 Moniflop Enabled!\r\n");
#endif

	GPIO_SetBits(SN74121_ENA_PORT, SN74121_ENA_PIN);
}

//================================
// �رմ�����
//================================
void SN74121_SetDisabled()
{
#ifdef _SET_DEBUG_MODE_
	UART_printf("  Set SN74121 Moniflop Disabled!\r\n");
#endif

	GPIO_ResetBits(SN74121_ENA_PORT, SN74121_ENA_PIN);
}
