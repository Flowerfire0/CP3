#ifndef __24CXX_H
#define __24CXX_H   
//////////////////////////////////////////////////////////////////////////////////	 
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#include "main.h"

#define	AT24C02_ADDR_WRITE	0xA0
#define	AT24C02_ADDR_READ	0xA1

uint8_t At24c02_Write_Byte(uint16_t addr, uint8_t* dat);
uint8_t At24c02_Read_Byte(uint16_t addr, uint8_t* read_buf);
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);
uint8_t AT24CXX_Check(void);
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);
#endif
















