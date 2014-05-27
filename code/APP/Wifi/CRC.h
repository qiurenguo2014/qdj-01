/**********************************************************************
* @File    : 	CRC.h
* @Author  : 	Summer
* @Version : 	V1.0
* @Date    :	2013.11.06
* @Brief   : 	ะฃั้
***********************************************************************/
#ifndef _CRC_H_
#define _CRC_H_

#include "datapacket.h"

void CRC_CreateNew();
void CRC_AddValue(UInt8 value);
UInt16 CRC_GetCRC16();
UInt16 CRC_CalcCRC16(UInt8* dataPacket, UInt16 length);

#endif
