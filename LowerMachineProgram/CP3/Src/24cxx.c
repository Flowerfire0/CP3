#include "24cxx.h" 
#include "i2c.h"

//////////////////////////////////////////////////////////////////////////////////	 							  
//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief		AT24C02�����ַдһ���ֽ�����
 * @param		addr ���� д���ݵĵ�ַ��0-255��
 * @param		dat  ���� ���д�����ݵĵ�ַ
 * @retval		�ɹ� ���� HAL_OK
*/
uint8_t At24c02_Write_Byte(uint16_t addr, uint8_t* dat)
{
	return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, dat, 1, 0xFFFF);
}

/**
 * @brief		AT24C02�����ַ��һ���ֽ�����
 * @param		addr ���� �����ݵĵ�ַ��0-255��
 * @param		read_buf ���� ��Ŷ�ȡ���ݵĵ�ַ
 * @retval		�ɹ� ���� HAL_OK
*/
uint8_t At24c02_Read_Byte(uint16_t addr, uint8_t* read_buf)
{
	return HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, addr, I2C_MEMADD_SIZE_8BIT, read_buf, 1, 0xFFFF);
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, ReadAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, NumToRead, 0xFFFF);

}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, NumToWrite, 0xFFFF);
}
 
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	uint8_t dat=0x55;

	At24c02_Read_Byte(255,&temp);			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		At24c02_Write_Byte(255,&dat);
			HAL_Delay(50);		//дһ�κͶ�һ��֮����Ҫ���ݵ���ʱ
	  At24c02_Read_Byte(255,&temp);		  
		if(temp==0X55)return 0;
	}
	return 1;											  
}


//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
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

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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







