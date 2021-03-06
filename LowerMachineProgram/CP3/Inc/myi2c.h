#ifndef _MYIIC_H
#define _MYIIC_H
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//IIC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#include "stdint.h"
#include "delay.h"
//IO方向设置
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}	//PB7输入模式
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;} 	//PB7输出模式

//IO操作
#define IIC_SCL(n) (n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n) (n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET)) //SDA
#define READ_SDA  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)  //输入SDA   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//IIC所有操作函数
void I2C1_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

