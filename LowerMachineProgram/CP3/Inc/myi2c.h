#ifndef _MYIIC_H
#define _MYIIC_H
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#include "stdint.h"
#include "delay.h"
//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}	//PB7����ģʽ
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;} 	//PB7���ģʽ

//IO����
#define IIC_SCL(n) (n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n) (n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET)) //SDA
#define READ_SDA  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)  //����SDA   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//IIC���в�������
void I2C1_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

