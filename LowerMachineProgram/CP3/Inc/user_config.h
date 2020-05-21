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

//0207-20191104 ����������λϵ����ˮѹ���������޸�
//0206  ���ӿ��Ź���can�������жϹؿ��������λ����can������
//0205	�޸Ĳ�ͷת�ٵĳ���Ϊ����
//0204 ɾ���ⲿ�жϵĶ�����䣬�ᵼ��ϵͳ����
//0203 ��������λϵ��ZWX,ADC�˲���can�ж����ȼ����
//0202 2019��8��19�� �޸������ٶȼ��㣬�ۼ�������㹫ʽ���ж�����
//0201 20190801 

#define MedianConsistency_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0,GPIO_PIN_RESET))
#define XZF_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1,GPIO_PIN_RESET))
#define XZB_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2,GPIO_PIN_RESET))
#define CTF_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,GPIO_PIN_RESET))
#define CTB_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4,GPIO_PIN_RESET))
#define ZW_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5,GPIO_PIN_RESET))
#define DC_Con(n) 	(n?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6,GPIO_PIN_RESET))

#define ReadPin_CTIN	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) 					//��ͷ���ŵ�ƽ
#define ReadPin_CTF1	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) 	
#define ReadPin_CTB1	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) 	
#define ReadPin_CTF2	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)  	
#define ReadPin_CTB2	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)  	
#define ReadPin_CTF3	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)  	
#define ReadPin_CTB3	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)  
#define ReadPin_FanSpeed	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define ReadPin_WalkSpeed	HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)

#define  MAX_AD_NUM   5			     //����5��ADCͨ��
/*      4-20ma,��Ӧ0-1MPa ,����165ŷķ��y=ax-b,a=0.305��b=249.8*/	   //ˮѹ������
/*      4-20ma,��Ӧ0-1MPa ,����150ŷķ��y=ax-b,a=0.3357��b=250*/	  //ˮѹ������
/*      4-20ma,��Ӧ0-1MPa ,����120ŷķ��y=ax-b,a=0.4196��b=250*/	  //ˮѹ������
/*      4-20ma,��Ӧ0-20MPa ,����150ŷķ��y=ax-b,a=6.7��b=5002*/	//��ѹ������   (����汾û��)

#define  Relation_Water_Pres  0.4196
#define  Water_Pres_b	   250.0
#define  Relation_Oil_Pres  6.7
#define  Oil_Pres_b	   5002.0

typedef struct {

	uint8_t XRM1Flag,XRM2Flag,XRM3Flag,LHQ1Flag,LHQ2Flag,LHQ3Flag ,LHQ1Flag1,LHQ2Flag2,LHQ3Flag3;
	uint8_t NeutralBit;//�յ�
	uint8_t Median;//��λ
	uint8_t Median_flag;
	uint8_t Profile;//����
	uint8_t Locked;//����
	uint8_t smooth;//��
	uint8_t CottonBoxFull;//����
	uint8_t CottonBoxLow;//�޹ܵ�λ
	uint8_t CottonBoxHigh;//�޹ܸ�λ
	uint8_t MedianController;//��λ����
	uint8_t ReversingController;//��������
	uint8_t HeadAuto;//��ͷ�Զ�����ģʽ
	uint8_t HeadCombinne;//��ͷ���
	
	uint32_t Water_Pres;//ˮѹ
	uint32_t Oil_Temp;
	uint32_t ControllerHandle;//����ֱ�
	uint32_t HeadPump;//��ͷ��
	uint32_t TravelPump;//���߱�
	uint16_t Fan_speed,Fan_speedB,Forward_speed;
	uint16_t HeadPumpInSpeed;
	uint16_t HeadPumpFSpeed1,HeadPumpFSpeed2,HeadPumpFSpeed3;
	uint16_t HeadPumpBSpeed1,HeadPumpBSpeed2,HeadPumpBSpeed3;	
	
} CP3_PARAMETER;

extern CP3_PARAMETER cp3_parameter;
extern CP3_PARAMETER cp3_parameter_old;

typedef struct{
	
	uint16_t P_CTIN_Speed_Counter;                 //��ͷ�ߵ�ƽ���� �����Ǽ�������Ч��ƽ
	uint16_t P_CTIN_Speed_Value;									 //��ͷ�����ڵ�ƽֵ
  uint16_t CTIN_SpeedCounter;                   //ÿ��������	
  uint16_t CTIN_SpeedPulse;                    //�����ƽֻ��һ�Σ���¼�ϴε�ƽ	

	uint16_t P_CTF1_Speed_Counter;              
	uint16_t P_CTF1_Speed_Value;		
  uint16_t CTF1_SpeedCounter;                   //ÿ��������	
  uint16_t CTF1_SpeedPulse; 
	
	uint16_t P_CTB1_Speed_Counter;              
	uint16_t P_CTB1_Speed_Value;	
  uint16_t CTB1_SpeedCounter;                   //ÿ��������	
  uint16_t CTB1_SpeedPulse; 
	
	uint16_t P_CTF2_Speed_Counter;              
	uint16_t P_CTF2_Speed_Value;	
  uint16_t CTF2_SpeedCounter;                   //ÿ��������	
  uint16_t CTF2_SpeedPulse; 
	
	uint16_t P_CTB2_Speed_Counter;              
	uint16_t P_CTB2_Speed_Value;		
  uint16_t CTB2_SpeedCounter;                   //ÿ��������	
  uint16_t CTB2_SpeedPulse; 

	uint16_t P_CTF3_Speed_Counter;              
	uint16_t P_CTF3_Speed_Value;	
  uint16_t CTF3_SpeedCounter;                   //ÿ��������	
  uint16_t CTF3_SpeedPulse; 
	
	uint16_t P_CTB3_Speed_Counter;              
	uint16_t P_CTB3_Speed_Value;	
	uint16_t CTB3_SpeedCounter;                   //ÿ��������	
  uint16_t CTB3_SpeedPulse; 
	
	uint16_t P_Fan_Speed_Counter;              
	uint16_t P_Fan_Speed_Value;	
	uint16_t Fan_SpeedCounter;                   //ÿ��������	
  uint16_t Fan_SpeedPulse;	
	
	uint16_t P_Walk_Speed_Counter;              
	uint16_t P_Walk_Speed_Value;	
	uint16_t Walk_SpeedCounter;                   //ÿ��������	
  uint16_t Walk_SpeedPulse;		
			
}P_SPEED_VARIABLE ;                       //��ͷת�ټ�����м����

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

extern __IO  uint16_t AD_Value[MAX_AD_NUM];     /*����һ�����飬�������ADC M��ͨ����ת�������Ҳ��DMA��Ŀ���ַ*/ 
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
