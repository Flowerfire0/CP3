#include "24CXX.h"

const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	
u8 datatemp[SIZE];

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}


///*---------读EEPROM--------------*/
//void ReadFromEEPROM(void)
//{
//	u8 temp;
//	union data1  { u32 f;  u8 g[4]; }h0={0};
//	union data3  { u32 f;  u8 g[4]; }h1={0};
//	union data5  { u32 f;  u8 g[4]; }h2={0};
//	union data7  { u32 f;  u8 g[4]; }h3={0}; //合并uint->float
//	union data9  { u32 f;  u8 g[4]; }h4={0};
//	union data11 { u32 f;  u8 g[4]; }h5={0};  
//	union data13 { u32 f;  u8 g[4]; }h6={0};
//	union data15 { u32 f;  u8 g[4]; }h7={0};
//	union data17 { u32 f;  u8 g[4]; }h8={0};
//	union data19 { u32 f;  u8 g[4]; }h9={0};
//	union data21 { u32 f;  u8 g[4]; }h10={0};
//  union data23 { u32 f;  u8 g[4]; }h11={0};
//	union data25 { u32 f;  u8 g[4]; }h12={0};
//	union data27 { u32 f;  u8 g[4]; }h13={0};
//	union data29 { u32 f;  u8 g[4]; }h14={0};
//	union data31 { u32 f;  u8 g[4]; }h15={0};
//	union data33 { u32 f;  u8 g[4]; }h16={0};
////	union data35 { u32 f;  u8 g[4]; }h17={0};	
////	union data37 { u32 f;  u8 g[4]; }h18={0};

//	
//	AT24CXX_Read(EEP_Firstpage,I2c_Buf_Read1,68);//从EEprom里读取6个float数据（4*18=76）
//	
//	h0.g[0]=I2c_Buf_Read1[0];  h0.g[1]=I2c_Buf_Read1[1];  h0.g[2]=I2c_Buf_Read1[2];  h0.g[3]=I2c_Buf_Read1[3];
//	h1.g[0]=I2c_Buf_Read1[4];  h1.g[1]=I2c_Buf_Read1[5];  h1.g[2]=I2c_Buf_Read1[6];  h1.g[3]=I2c_Buf_Read1[7];
//	h2.g[0]=I2c_Buf_Read1[8];  h2.g[1]=I2c_Buf_Read1[9];  h2.g[2]=I2c_Buf_Read1[10]; h2.g[3]=I2c_Buf_Read1[11];
//	h3.g[0]=I2c_Buf_Read1[12]; h3.g[1]=I2c_Buf_Read1[13]; h3.g[2]=I2c_Buf_Read1[14]; h3.g[3]=I2c_Buf_Read1[15];
//	h4.g[0]=I2c_Buf_Read1[16]; h4.g[1]=I2c_Buf_Read1[17]; h4.g[2]=I2c_Buf_Read1[18]; h4.g[3]=I2c_Buf_Read1[19];
//	h5.g[0]=I2c_Buf_Read1[20]; h5.g[1]=I2c_Buf_Read1[21]; h5.g[2]=I2c_Buf_Read1[22]; h5.g[3]=I2c_Buf_Read1[23];
//	h6.g[0]=I2c_Buf_Read1[24]; h6.g[1]=I2c_Buf_Read1[25]; h6.g[2]=I2c_Buf_Read1[26]; h6.g[3]=I2c_Buf_Read1[27];
//	h7.g[0]=I2c_Buf_Read1[28]; h7.g[1]=I2c_Buf_Read1[29]; h7.g[2]=I2c_Buf_Read1[30]; h7.g[3]=I2c_Buf_Read1[31];
//	h8.g[0]=I2c_Buf_Read1[32]; h8.g[1]=I2c_Buf_Read1[33];	h8.g[2]=I2c_Buf_Read1[34]; h8.g[3]=I2c_Buf_Read1[35];
//	h9.g[0]=I2c_Buf_Read1[36]; h9.g[1]=I2c_Buf_Read1[37]; h9.g[2]=I2c_Buf_Read1[38]; h9.g[3]=I2c_Buf_Read1[39];
//	h10.g[0]=I2c_Buf_Read1[40]; h10.g[1]=I2c_Buf_Read1[41];	h10.g[2]=I2c_Buf_Read1[42]; h10.g[3]=I2c_Buf_Read1[43];
//	h11.g[0]=I2c_Buf_Read1[44]; h11.g[1]=I2c_Buf_Read1[45];	h11.g[2]=I2c_Buf_Read1[46]; h11.g[3]=I2c_Buf_Read1[47];
//	h12.g[0]=I2c_Buf_Read1[48]; h12.g[1]=I2c_Buf_Read1[49];	h12.g[2]=I2c_Buf_Read1[50]; h12.g[3]=I2c_Buf_Read1[51];
//	h13.g[0]=I2c_Buf_Read1[52]; h13.g[1]=I2c_Buf_Read1[53];	h13.g[2]=I2c_Buf_Read1[54]; h13.g[3]=I2c_Buf_Read1[55];
//	h14.g[0]=I2c_Buf_Read1[56]; h14.g[1]=I2c_Buf_Read1[57];	h14.g[2]=I2c_Buf_Read1[58]; h14.g[3]=I2c_Buf_Read1[59];
//	h15.g[0]=I2c_Buf_Read1[60]; h15.g[1]=I2c_Buf_Read1[61];	h15.g[2]=I2c_Buf_Read1[62]; h15.g[3]=I2c_Buf_Read1[63];
//	h16.g[0]=I2c_Buf_Read1[64]; h16.g[1]=I2c_Buf_Read1[65];	h16.g[2]=I2c_Buf_Read1[66]; h16.g[3]=I2c_Buf_Read1[67];
////	h17.g[0]=I2c_Buf_Read1[68]; h17.g[1]=I2c_Buf_Read1[69];	h17.g[2]=I2c_Buf_Read1[70]; h17.g[3]=I2c_Buf_Read1[71];	
////	h18.g[0]=I2c_Buf_Read1[72]; h18.g[1]=I2c_Buf_Read1[73];	h18.g[2]=I2c_Buf_Read1[74]; h18.g[3]=I2c_Buf_Read1[75];
//	
////	Coeff_Walking_Speed_Can = h0.f; Coeff_FJS_Can = h1.f; 
//	Coeff_Land_Can = h2.f;  LHQ_ratio_Can = h3.f;
//	temp = h4.f; XZZX_Can = h5.f;  MachineID = h6.f;Coeff_head_Locked_Can = h7.f; 
//	InGearN = h8.f;FGearN = h9.f; BGearN = h10.f;HeadSpeed1 = h11.f;
//	HeadSpeed2=h12.f;HeadSpeed3 = h13.f; HeadSpeed4 = h14.f; HeadSpeed5 = h15.f; 	
//	HeadSpeed6 = h16.f; 	

//	if(temp>30 && temp<=100)
//		XZ_length_ratio_Can = temp;
//	else
//		XZ_length_ratio_Can = 100;
//		
//}


///*---------写参数信息到EEPROM--------------*/
///*  Coeff_Walking_Speed:行走速度系数；Alarm_Opened：蜂鸣器总开关；
////  Coeff_FJS：风机转速系数；   Coeff_Land：收获面积系数；Coeff_FDS：发动机转速系数；
//*/
//void WriteToEEPROM(void)
//{
//	uint8_t i;
//	union data0  { u32 b;  u8 a[4];}c0; 
//	union data2  { u32 b;  u8 a[4];}c1;
//	union data4  { u32 b;  u8 a[4];}c2;
//	union data6  { u32 b;  u8 a[4];}c3;
//	union data8  { u32 b;  u8 a[4];}c4;  
//	union data10 { u32 b;  u8 a[4];}c5;  //拆解float->uint
//	union data12 { u32 b;  u8 a[4];}c6;
//	union data14 { u32 b;  u8 a[4];}c7;
//	union data16 { u32 b;  u8 a[4];}c8;
//	union data18 { u32 b;  u8 a[4];}c9;
//	union data20 { u32 b;  u8 a[4];}c10;
//	union data22 { u32 b;  u8 a[4];}c11;
//	union data24 { u32 b;  u8 a[4];}c12;
//	union data26 { u32 b;  u8 a[4];}c13;
//	union data28 { u32 b;  u8 a[4];}c14;
//	union data30 { u32 b;  u8 a[4];}c15;	
//	union data32 { u32 b;  u8 a[4];}c16;
////	union data34 { u32 b;  u8 a[4];}c17;
////	union data36 { u32 b;  u8 a[4];}c18;
//	
//	u8* p0;  u8* p1; u8* p2; u8* p3; u8* p4; u8* p5;u8* p6;u8* p7;u8* p8;u8* p9;  u8* p10;  u8* p11;   u8* p12;   u8* p13;  u8* p14; u8* p15; u8* p16;

////	c0.b=Coeff_Walking_Speed_Can; c1.b=Coeff_FJS_Can; 
//	c2.b=Coeff_Land_Can; c3.b=LHQ_ratio_Can;
//	c4.b=XZ_length_ratio_Can; c5.b=XZZX_Can;c6.b=MachineID;c7.b=Coeff_head_Locked_Can;
//	c8.b=InGearN;c9.b=FGearN;c10.b=BGearN;c11.b=HeadSpeed1;
//	c12.b=HeadSpeed2;c13.b=HeadSpeed3; c14.b=HeadSpeed4; c15.b=HeadSpeed5;
//	c16.b=HeadSpeed6;

//	p0=c0.a;  p1=c1.a; p2=c2.a; p3=c3.a;p4=c4.a; p5=c5.a;p6=c6.a;p7=c7.a;p8=c8.a;p9=c9.a;p10=c10.a;p11=c11.a;p12=c12.a;p13=c13.a;p14=c14.a;p15=c15.a;p16=c16.a;
//	for(i=0;i<4;i++) {I2c_Buf_Write1[i]=*p0;  p0++;}
//	for(i=4;i<8;i++) {I2c_Buf_Write1[i]=*p1; p1++;}
//	for(i=8;i<12;i++) {I2c_Buf_Write1[i]=*p2; p2++;}
//	for(i=12;i<16;i++) {I2c_Buf_Write1[i]=*p3; p3++;}
//	for(i=16;i<20;i++) {I2c_Buf_Write1[i]=*p4; p4++;}
//	for(i=20;i<24;i++) {I2c_Buf_Write1[i]=*p5; p5++;}
//	for(i=24;i<28;i++) {I2c_Buf_Write1[i]=*p6; p6++;}
//	for(i=28;i<32;i++) {I2c_Buf_Write1[i]=*p7; p7++;}
//	for(i=32;i<36;i++) {I2c_Buf_Write1[i]=*p8; p8++;}	
//	for(i=36;i<40;i++) {I2c_Buf_Write1[i]=*p9; p9++;}
//	for(i=40;i<44;i++) {I2c_Buf_Write1[i]=*p10; p10++;}	
//	for(i=44;i<48;i++) {I2c_Buf_Write1[i]=*p11; p11++;}	
//	for(i=48;i<52;i++) {I2c_Buf_Write1[i]=*p12; p12++;}	
//	for(i=52;i<56;i++) {I2c_Buf_Write1[i]=*p13; p13++;}	
//	for(i=56;i<60;i++) {I2c_Buf_Write1[i]=*p14; p14++;}
//	for(i=60;i<64;i++) {I2c_Buf_Write1[i]=*p15; p15++;}	
//	for(i=64;i<68;i++) {I2c_Buf_Write1[i]=*p16; p16++;}	
////	for(i=68;i<72;i++) {I2c_Buf_Write1[i]=*p17; p17++;}
////	for(i=72;i<76;i++) {I2c_Buf_Write1[i]=*p18; p18++;}
//	
//	AT24CXX_Write(EEP_Firstpage,(u8*)I2c_Buf_Write1,68);//往EEprom里写入数据

//}

///*---------写收获面积信息到EEPROM--------------*/
//void WriteLandToEEPROM(void)
//{
//	uint8_t i;
//	union LandData0 { u32 b;  u8 f[4];}r0; 
//	union LandData2 { u32 b;  u8 f[4];}r1; //拆解float->uint
//	union _init { u32 b;  u8 f[4];}r2; 
//	u8* p0;  u8* p1;  u8* p2;

//	r0.b=HarvestedLand; r1.b=TotalHarvestedLand;
//	r2.b=(uint8_t)_flag_init;
//	p0=r0.f;  p1=r1.f;  p2=r2.f;
//	for(i=0;i<4;i++) {I2c_Buf_Write2[i]=*p0;  p0++;}	//	HarvestedLand
//	for(i=4;i<8;i++) {I2c_Buf_Write2[i]=*p1; p1++;}	//	TotalHarvestedLand
//	for(i=8;i<12;i++) {I2c_Buf_Write2[i]=*p2; p2++;}
//	AT24CXX_Write(EEP_Secondpage,I2c_Buf_Write2,12);//往EEprom里写入数据
//}

///*---------从EEPROM读取收获面积信息--------------*/
//void ReadLandFromEEPROM(void)
//{
//	union LandData1 { u32 f;  u8 q[4]; }y0={0};
//	union LandData3 { u32 f;  u8 q[4]; }y1={0};    //合并uint->float
//	union LandData5 { u32 f;  u8 q[4]; }y2={0};

//	AT24CXX_Read(EEP_Secondpage,I2c_Buf_Read2,12);//从EEprom里读取2个float数据（4*2=8）
//	y0.q[0]=I2c_Buf_Read2[0];  y0.q[1]=I2c_Buf_Read2[1];  y0.q[2]=I2c_Buf_Read2[2];  y0.q[3]=I2c_Buf_Read2[3];
//	y1.q[0]=I2c_Buf_Read2[4];  y1.q[1]=I2c_Buf_Read2[5];  y1.q[2]=I2c_Buf_Read2[6];  y1.q[3]=I2c_Buf_Read2[7];
//	y2.q[0]=I2c_Buf_Read2[8];  y2.q[1]=I2c_Buf_Read2[9];  y2.q[2]=I2c_Buf_Read2[10];  y2.q[3]=I2c_Buf_Read2[11];

//    HarvestedLand=y0.f; TotalHarvestedLand= y1.f; _flag_init = y2.f;
//	  TotalHarvestedLand_old= y1.f;
//}

