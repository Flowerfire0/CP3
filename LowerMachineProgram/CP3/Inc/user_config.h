#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#include <stdio.h>
#include <usart.h>
#include "math.h"
#include "delay.h"

#define version1 3//0302
#define version2 3
#define VerYear 20//20200402
#define VerMonth 04
#define VerDay 27

//0207-20191104 增加行走限位系数，水压报警条件修改
//0206  增加看门狗，can发送总中断关开，软件复位放在can接收里
//0205	修改采头转速的齿数为变量
//0204 删除外部中断的多余语句，会导致系统死机
//0203 增加了中位系数ZWX,ADC滤波，can中断优先级提高
//0202 2019年8月19日 修改行走速度计算，累计面积计算公式和判断条件
//0201 20190801 

#define MedianConsistency_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0,GPIO_PIN_RESET))
#define XZF_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1,GPIO_PIN_RESET))
#define XZB_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2,GPIO_PIN_RESET))
#define CTF_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,GPIO_PIN_RESET))
#define CTB_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_RESET))
#define ZW_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_RESET))
#define DC_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_RESET))

#define ReadPin_CTIN	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) 					//采头引脚电平
#define ReadPin_CTF1	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) 	
#define ReadPin_CTB1	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) 	
#define ReadPin_CTF2	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)  	
#define ReadPin_CTB2	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)  	
#define ReadPin_CTF3	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)  	
#define ReadPin_CTB3	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)  
#define ReadPin_FanSpeed	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define ReadPin_WalkSpeed	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)

#define  MAX_AD_NUM   5			     //开启5个ADC通道
/*      4-20ma,对应0-1MPa ,电阻165欧姆，y=ax-b,a=0.305，b=249.8*/	   //水压传感器
/*      4-20ma,对应0-1MPa ,电阻150欧姆，y=ax-b,a=0.3357，b=250*/	  //水压传感器
/*      4-20ma,对应0-1MPa ,电阻120欧姆，y=ax-b,a=0.4196，b=250*/	  //水压传感器
/*      4-20ma,对应0-20MPa ,电阻150欧姆，y=ax-b,a=6.7，b=5002*/	//油压传感器   (这个版本没用)

#define  Relation_Water_Pres  0.4196
#define  Water_Pres_b	   250.0
#define  Relation_Oil_Pres  6.7
#define  Oil_Pres_b	   5002.0

typedef struct {

	uint8_t XRM1Flag,XRM2Flag,XRM3Flag,LHQ1Flag,LHQ2Flag,LHQ3Flag ,LHQ1Flag1,LHQ2Flag2,LHQ3Flag3;
	uint8_t NeutralBit;//空挡
	uint8_t Median;//中位
	uint8_t Median_flag;
	uint8_t Profile;//仿形
	uint8_t Locked;//锁定
	uint8_t smooth;//润滑
	uint8_t CottonBoxFull;//箱满
	uint8_t CottonBoxLow;//棉管低位
	uint8_t CottonBoxHigh;//棉管高位
	uint8_t MedianController;//中位控制
	uint8_t ReversingController;//倒车控制
	uint8_t HeadAuto;//采头自动控制模式
	uint8_t HeadCombinne;//采头结合
	
	uint32_t Water_Pres;//水压
	uint32_t Oil_Temp;
	uint32_t ControllerHandle;//电控手柄
	uint32_t HeadPump;//采头泵
	uint32_t TravelPump;//行走泵
	uint16_t Fan_speed,Fan_speedB,Forward_speed;
	uint16_t HeadPumpInSpeed;
	uint16_t HeadPumpFSpeed1,HeadPumpFSpeed2,HeadPumpFSpeed3;
	uint16_t HeadPumpBSpeed1,HeadPumpBSpeed2,HeadPumpBSpeed3;	
	
} CP3_PARAMETER;

extern CP3_PARAMETER cp3_parameter;
extern CP3_PARAMETER cp3_parameter_old;

typedef struct{
	
	uint16_t P_CTIN_Speed_Counter;                 //采头高电平计数 连续记几次算有效电平
	uint16_t P_CTIN_Speed_Value;									 //采头上周期电平值
  uint16_t CTIN_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTIN_SpeedPulse;                    //脉冲电平只记一次，纪录上次电平	

	uint16_t P_CTF1_Speed_Counter;              
	uint16_t P_CTF1_Speed_Value;		
  uint16_t CTF1_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTF1_SpeedPulse; 
	
	uint16_t P_CTB1_Speed_Counter;              
	uint16_t P_CTB1_Speed_Value;	
  uint16_t CTB1_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTB1_SpeedPulse; 
	
	uint16_t P_CTF2_Speed_Counter;              
	uint16_t P_CTF2_Speed_Value;	
  uint16_t CTF2_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTF2_SpeedPulse; 
	
	uint16_t P_CTB2_Speed_Counter;              
	uint16_t P_CTB2_Speed_Value;		
  uint16_t CTB2_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTB2_SpeedPulse; 

	uint16_t P_CTF3_Speed_Counter;              
	uint16_t P_CTF3_Speed_Value;	
  uint16_t CTF3_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTF3_SpeedPulse; 
	
	uint16_t P_CTB3_Speed_Counter;              
	uint16_t P_CTB3_Speed_Value;	
	uint16_t CTB3_SpeedCounter;                   //每秒脉冲数	
  uint16_t CTB3_SpeedPulse; 
	
	uint16_t P_Fan_Speed_Counter;              
	uint16_t P_Fan_Speed_Value;	
	uint16_t Fan_SpeedCounter;                   //每秒脉冲数	
  uint16_t Fan_SpeedPulse;	
	
	uint16_t P_Walk_Speed_Counter;              
	uint16_t P_Walk_Speed_Value;	
	uint16_t Walk_SpeedCounter;                   //每秒脉冲数	
  uint16_t Walk_SpeedPulse;		
			
}P_SPEED_VARIABLE ;                       //采头转速计算的中间变量

extern P_SPEED_VARIABLE  p_speed_variable ;

extern uint16_t CTFS1,CTFS2,CTFS3;
extern uint16_t CTBS1,CTBS2,CTBS3;
extern uint8_t WarnFlag;
extern uint8_t AlarmStatusDate0,AlarmStatusDate1,AlarmStatusDate2,AlarmStatusDate3 ,AlarmStatusDate4;
extern uint8_t E0_HeartBeat;
extern uint16_t MachineID;
extern uint32_t HarvestedLand,TotalHarvestedLand,TotalHarvestedLand_old;
extern float Coeff_Land;
extern float Coeff_head_Locked ,Coeff_Travel_head_ratio,LHQ_ratio ,CT_length_ratio,XZ_length_ratio;
extern uint8_t WaterP_Alarm; 
extern uint8_t OilP_Alarm;
extern uint16_t InGearN,FGearN,BGearN;
extern float CTZX,XZZX;

extern __IO  uint16_t AD_Value[MAX_AD_NUM];     /*定义一个数组，用来存放ADC M个通道的转换结果，也是DMA的目标地址*/ 
extern uint32_t sum_Water_Pres;
extern uint32_t sum_OilTemp;
extern uint32_t sum_ControllerHandle;
extern uint32_t sum_TravelPump;
extern uint32_t sum_HeadPump;
extern uint8_t Water_Sensor_Break,Water_Sensor_Short;

extern uint16_t Coeff_Land_Can,Coeff_head_Locked_Can,LHQ_ratio_Can,CT_length_ratio_Can,XZ_length_ratio_Can,CTZX_Can,XZZX_Can;
extern uint16_t BackTransmissionRatio_Can,FrontTransmissionRatio_Can;
extern uint16_t XZS_Disp,FJS_Disp,CTFS_Disp1,CTBS_Disp1,CTFS_Disp2,CTBS_Disp2,CTFS_Disp3,CTBS_Disp3,CTS_Disp;
extern uint16_t Water_Pres_Avr,ControllerHandle_Avr,TravelPump_Avr,HeadPump_Avr;
extern uint16_t CTS_SET1,CTS_SET2,CTS_SET3,CTS_SET4,CTS_SET5,CTS_SET6;
extern uint8_t FJ_Alarm;
extern uint16_t CTS_set;
extern uint8_t Flag_Median,flag_Reverse;
extern uint16_t OilTempAlarm;

void TravelControl(void);
void HeadControl(void);

#endif
