#include "24cxx.h" 
#include "i2c.h"

//////////////////////////////////////////////////////////////////////////////////	 							  
//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief		AT24C02任意地址写一个字节数据
 * @param		addr —— 写数据的地址（0-255）
 * @param		dat  —— 存放写入数据的地址
 * @retval		成功 —— HAL_OK
*/
uint8_t At24c02_Write_Byte(uint16_t addr, uint8_t* dat)
{
	return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, dat, 1, 0xFFFF);
}

/**
 * @brief		AT24C02任意地址读一个字节数据
 * @param		addr —— 读数据的地址（0-255）
 * @param		read_buf —— 存放读取数据的地址
 * @retval		成功 —— HAL_OK
*/
uint8_t At24c02_Read_Byte(uint16_t addr, uint8_t* read_buf)
{
	return HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, addr, I2C_MEMADD_SIZE_8BIT, read_buf, 1, 0xFFFF);
}


//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, ReadAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, NumToRead, 0xFFFF);

}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, NumToWrite, 0xFFFF);
}
 
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	uint8_t dat=0x55;

	At24c02_Read_Byte(255,&temp);			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		At24c02_Write_Byte(255,&dat);
			HAL_Delay(50);		//写一次和读一次之间需要短暂的延时
	  At24c02_Read_Byte(255,&temp);		  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	uint8_t dat;
	for(t=0;t<Len;t++)
	{
		dat=(uint8_t)DataToWrite>>(8*t);
		At24c02_Write_Byte(WriteAddr+t,&dat);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t;
	uint8_t dat;
	uint32_t temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		At24c02_Read_Byte(ReadAddr+Len-t-1,&dat);
		temp+=dat; 	 				   
	}
	return temp;												    
}







