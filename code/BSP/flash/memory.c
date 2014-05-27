/*========================================================================
#   FileName: memory.c
#     Author: Allen
#      Email: qiurenguo@gmail.com
#   HomePage: Allen
# LastChange: 2013-10-30 11:21:35
========================================================================*/
/* Includes ---------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "../fwlib/flash.h"
#include "memory.h"
/* Types ------------------------------------------------------------*/
/* Constants --------------------------------------------------------*/
/* Define -----------------------------------------------------------*/
/* Variables --------------------------------------------------------*/
static uint32_t flash_buf[FLASH_PAGE_SIZE/4];
static uint32_t sysflag;
/* Functions prototypes ---------------------------------------------*/
static uint32_t _MM_ErasePage (uint32_t Page);
static uint32_t _MM_ProgramWord (uint32_t Address, uint32_t Data);
static uint32_t _MM_ReadWord (uint32_t Address);
static uint16_t _MM_ReadHalfWord (uint32_t Address);
static uint8_t _MM_ReadByte (uint32_t Address);
void _MM_PFlashSwapCtl (uint8_t blockx);
void MM_EnableAllIRQ(void);
void MM_DisableAllIRQ (void);
void f1 (void);
/* Functions --------------------------------------------------------*/
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void MM_Configuration (void)
{
	FLASH_Init ();
	sysflag = MM_ReadLWord (OFFSET_SYSTEM);
}
void MM_ResumeSysflag (void)
{
	MM_ProgramLWord (OFFSET_SYSTEM,0xffff);
}
void MM_SetSysFlag (uint32_t sysparam)
{
	sysflag |= sysparam;
	MM_ProgramLWord (OFFSET_SYSTEM,sysflag);
}
void MM_ClrSysFlag (uint32_t sysparam)
{
	sysflag &= ~sysparam;
	MM_ProgramLWord (OFFSET_SYSTEM,sysflag);
}
void f1 (void)
{
	_MM_ProgramWord (FLASH_START_ADDR+FLASH_PAGE_SIZE, 0xaa5555aaul);
	_MM_ErasePage (FLASH_START_ADDR/2048);
	printf ("read0:%x\r\n",_MM_ReadWord(FLASH_START_ADDR));
	_MM_ProgramWord (FLASH_START_ADDR, 0xffff55aaul);
	printf ("read1:%x\r\n",_MM_ReadWord(FLASH_START_ADDR));		
	printf ("read2:%x\r\n",_MM_ReadWord(FLASH_START_ADDR+FLASH_PAGE_SIZE));
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t MM_ReadLWord (uint32_t offset)
{
	return (*(uint32_t *)(FLASH_START_ADDR+offset));
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t MM_ProgramLWord (const uint32_t offset, const uint32_t data)
{
	uint32_t flash_status = FLASH_OK;
	uint32_t Address = FLASH_START_ADDR + offset;
	uint32_t i;
	uint32_t section_addr;
	section_addr = offset/FLASH_PAGE_SIZE;
	section_addr *= FLASH_PAGE_SIZE;
	for (i=0; i<FLASH_PAGE_SIZE/4; i++){
		flash_buf [i] = MM_ReadLWord (section_addr+i*4);
	}
	MM_DisableAllIRQ ();
	FTFL->FCCOB0 = ERSSCR;
	FTFL->FCCOB1 = (uint8_t)((FLASH_START_ADDR+section_addr)>>16);
	FTFL->FCCOB2 = (uint8_t)((FLASH_START_ADDR+section_addr)>>8);
	FTFL->FCCOB3 = (uint8_t)(FLASH_START_ADDR+section_addr);
    FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    // 启动命令
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    // 等待命令结束
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0){
	}

	flash_buf[(offset%FLASH_PAGE_SIZE)/4] = data;
	while (!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)) {};
	FTFL->FCCOB0 = PGM4;
	for (i=0; i<FLASH_PAGE_SIZE/4; i++){
		FTFL->FCCOB1 = (uint8_t)((FLASH_START_ADDR+section_addr)>>16);
		FTFL->FCCOB2 = (uint8_t)((FLASH_START_ADDR+section_addr)>>8);
		FTFL->FCCOB3 = (uint8_t)((FLASH_START_ADDR+section_addr));

		FTFL->FCCOB4 = (uint8_t)(flash_buf[i] >> 24);
		FTFL->FCCOB5 = (uint8_t)(flash_buf[i] >> 16);
		FTFL->FCCOB6 = (uint8_t)(flash_buf[i] >> 8);
		FTFL->FCCOB7 = (uint8_t)(flash_buf[i]);
		section_addr += 4;
		FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK
						|FTFL_FSTAT_RDCOLERR_MASK);
    	FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    	while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);
    	if(FTFL->FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK
							|FTFL_FSTAT_MGSTAT0_MASK
							|FTFL_FSTAT_RDCOLERR_MASK)){
			flash_status = FLASH_ERROR;
		}
	}
	MM_EnableAllIRQ ();
	return flash_status;
}
/**
  * @brief  由于在写flash时，不关闭中断，将引起芯片复位.
  * @param  None
  * @retval None
  */
void MM_DisableAllIRQ (void)
{
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
void MM_EnableAllIRQ (void)
{
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t _MM_ErasePage (uint32_t Page)
{
	uint32_t flash_status=FLASH_OK;
	uint32_t PageAddress = Page*(1<<11);	
	FTFL->FCCOB0 = ERSSCR;
	FTFL->FCCOB1 = (uint8_t)((PageAddress>>16)&0xff);
	FTFL->FCCOB2 = (uint8_t)((PageAddress>>8)&0xff);
	FTFL->FCCOB3 = (uint8_t)(PageAddress&0xff);

    FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    // 启动命令
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    // 等待命令结束
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0){
	}

    if(FTFL->FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_MGSTAT0_MASK|FTFL_FSTAT_RDCOLERR_MASK))
		flash_status = FLASH_ERROR;

	return flash_status;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t _MM_EraseAllPage (void)
{
	uint32_t flash_status = 0;
	return flash_status;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t _MM_ProgramWord (uint32_t Address, uint32_t Data)
{
	uint32_t flash_status = FLASH_OK;
//	while (!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)) {};
	FTFL->FCCOB0 = PGM4;
	FTFL->FCCOB1 = (uint8_t)(Address>>16 & 0xff);
	FTFL->FCCOB2 = (uint8_t)(Address>>8 & 0xff);
	FTFL->FCCOB3 = (uint8_t)(Address & 0xff);

	FTFL->FCCOB4 = (uint8_t)(Data >> 24 & 0xff);
	FTFL->FCCOB5 = (uint8_t)(Data >> 16 & 0xff);
	FTFL->FCCOB6 = (uint8_t)(Data >> 8 & 0xff);
	FTFL->FCCOB7 = (uint8_t)(Data & 0xff);
	
	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);

    if(FTFL->FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_MGSTAT0_MASK|FTFL_FSTAT_RDCOLERR_MASK))
		flash_status = FLASH_ERROR;
	return flash_status;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t _MM_ProgramHalfWord (uint32_t Address, uint16_t Data)
{
	uint32_t flash_status = 0;
	return flash_status;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t _MM_ProgramByge (uint32_t Address, uint8_t Data)
{
	uint32_t flash_status = 0;
	return flash_status;
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint32_t _MM_ReadWord (uint32_t Address)
{
	return (*(uint32_t *)(Address));
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint16_t _MM_ReadHalfWord (uint32_t Address)
{
	return *(uint16_t *)(Address);
}
/**
  * @brief  .
  * @param  None
  * @retval None
  */
uint8_t _MM_ReadByte (uint32_t Address)
{
	return *(uint8_t *)(Address); 
}
void _MM_PFlashSwapCtl (uint8_t blockx)
{
	_MM_ProgramWord (FLASH_START_ADDR, 0xffff55aaul);
	//printf ("read1:%x\r\n",_MM_ReadWord(FLASH_START_ADDR));	
	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
	FTFL->FCCOB0 = 0x46;
	FTFL->FCCOB1 = (uint8_t)(0x0);
	FTFL->FCCOB2 = (uint8_t)(0x0);
	FTFL->FCCOB3 = (uint8_t)(0x0);

	FTFL->FCCOB4 = (uint8_t)(0x01);
	FTFL->FCCOB5 = (uint8_t)(0xff);
	FTFL->FCCOB6 = (uint8_t)(0xff);
	FTFL->FCCOB7 = (uint8_t)(0xff);
	
	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);
	
		FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
	FTFL->FCCOB0 = 0x46;
	FTFL->FCCOB1 = (uint8_t)(0x0);
	FTFL->FCCOB2 = (uint8_t)(0x0);
	FTFL->FCCOB3 = (uint8_t)(0x0);

	FTFL->FCCOB4 = (uint8_t)(0x02);
	FTFL->FCCOB5 = (uint8_t)(0xff);
	FTFL->FCCOB6 = (uint8_t)(0xff);
	FTFL->FCCOB7 = (uint8_t)(0xff);
	
	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);

	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
	FTFL->FCCOB0 = 0x46;
	FTFL->FCCOB1 = (uint8_t)(0x0);
	FTFL->FCCOB2 = (uint8_t)(0x0);
	FTFL->FCCOB3 = (uint8_t)(0x0);

	FTFL->FCCOB4 = (uint8_t)(0x04);
	FTFL->FCCOB5 = (uint8_t)(0xff);
	FTFL->FCCOB6 = (uint8_t)(0xff);
	FTFL->FCCOB7 = (uint8_t)(0xff);
	
	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);
	
		FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
	FTFL->FCCOB0 = 0x46;
	FTFL->FCCOB1 = (uint8_t)(0x0);
	FTFL->FCCOB2 = (uint8_t)(0x0);
	FTFL->FCCOB3 = (uint8_t)(0x0);

	FTFL->FCCOB4 = (uint8_t)(0x08);
	FTFL->FCCOB5 = (uint8_t)(0xff);
	FTFL->FCCOB6 = (uint8_t)(0xff);
	FTFL->FCCOB7 = (uint8_t)(0xff);
	
	FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);
	//_MM_ProgramWord (FLASH_START_ADDR*2, 0xffff55aaul);
	//printf ("read1:%x\r\n",_MM_ReadWord(FLASH_START_ADDR*2));	
//	printf ("b:%u,%u,%u\r\n",FTFL->FCCOB5,FTFL->FCCOB6,FTFL->FCCOB7);
}
/*********************************END OF FILE**********************************/


