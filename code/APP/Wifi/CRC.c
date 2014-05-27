/**********************************************************************
* @File    : 	CRC.cpp
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.06
* @Brief   : 	ะฃั้
***********************************************************************/
#include "CRC.h"

static UInt16 glbCRC16 = 0;

UInt16 CRC_CalcCRC16(UInt8* dataPacket, UInt16 length)
{
	UInt16 crc = 0xFFFF;
	int cnt;
	while(length--) {
		crc ^= *(dataPacket++);

		for(cnt = 0; cnt < 8; cnt++) {
			if((crc & 0x01) != 0) {
				crc >>= 1;
				crc ^= 0xA001;
			} else {
				crc >>= 1;
			}
		}
	}

	return crc;
}

void CRC_CreateNew()
{
	glbCRC16 = 0xFFFF;
}

void CRC_AddValue(UInt8 value)
{
	int cnt;
	glbCRC16 ^= value;

	for(cnt = 0; cnt < 8; cnt++) {
		if((glbCRC16 & 0x01) != 0) {
			glbCRC16 >>= 1;
			glbCRC16 ^= 0xA001;
		} else {
			glbCRC16 >>= 1;
		}
	}
}

UInt16 CRC_GetCRC16()
{
	return glbCRC16;
}
