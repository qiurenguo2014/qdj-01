/**********************************************************************
* @File    : 	SN74121.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.10
* @Brief   : 	单稳态触发器驱动
***********************************************************************/
#include "SN74121.h"


//* Private ---------------------------------------------------------*//

//================================
// 初始化配置
//================================
static void sGPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// 配置 SN74121_EN 为输出口
	GPIO_InitStruct.GPIOx = SN74121_ENA_PORT;
	GPIO_InitStruct.GPIO_InitState = Bit_RESET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OPP;
	GPIO_InitStruct.GPIO_Pin = SN74121_ENA_PIN;
	GPIO_Init(&GPIO_InitStruct);
	// 配置 SN74121_EXT 为浮空输入，下降沿中断
	GPIO_InitStruct.GPIOx = SN74121_EXT_PORT;
	GPIO_InitStruct.GPIO_InitState = Bit_SET;
	GPIO_InitStruct.GPIO_IRQMode = GPIO_IT_FALLING;
	GPIO_InitStruct.GPIO_Pin = SN74121_EXT_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(&GPIO_InitStruct);
}


//* Public ---------------------------------------------------------*//

//================================
// 初始化单稳态触发器
//================================
void SN74121_Init()
{
#ifdef _SET_DEBUG_MODE_
	UART_printf("Init SN74121 Moniflop...\t\t\t");
#endif

	//初始化端口
	sGPIO_Configuration();

	NVIC_EnableIRQ(SN74121_EXT_IRQ);

#ifdef _SET_DEBUG_MODE_
	UART_printf("[OK]\r\n");
#endif
}

//================================
// 开启触发器
//================================
void SN74121_SetEnabled()
{
#ifdef _SET_DEBUG_MODE_
	UART_printf("  Set SN74121 Moniflop Enabled!\r\n");
#endif

	GPIO_SetBits(SN74121_ENA_PORT, SN74121_ENA_PIN);
}

//================================
// 关闭触发器
//================================
void SN74121_SetDisabled()
{
#ifdef _SET_DEBUG_MODE_
	UART_printf("  Set SN74121 Moniflop Disabled!\r\n");
#endif

	GPIO_ResetBits(SN74121_ENA_PORT, SN74121_ENA_PIN);
}
