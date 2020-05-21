#include "DataCollection.h"

CP3_PARAMETER cp3_parameter;
CP3_PARAMETER cp3_parameter_old;
P_SPEED_VARIABLE  p_speed_variable ;


uint8_t AlarmFlag=0;	//�ܵĹ��ϣ�����LED����ʾ��0=����������1=��������
uint8_t FaultFlag=0,WarnFlag=0; // 00=������01=���棬10=����, ����BoxFull_WarnFlag��־

uint8_t AlarmStatusDate0,AlarmStatusDate1,AlarmStatusDate2,AlarmStatusDate3 ,AlarmStatusDate4;
uint8_t FJ_Alarm = 0,WaterP_Alarm = 0; /*ϵͳ��λ����������������ܿ���*/
uint8_t OilP_Alarm = 0;

uint16_t  FJS_Disp=0;	      //Fan_speed����������������,FJSΪÿ���ӽ��յ�����������FJS_Disp����ȥ��ʾ��ֵ
uint16_t  XZS_Disp=0;	//Forward_speed�����ٶȴ������������,XZSΪÿ���ӽ��յ�����������XZS_Disp������ȥ��ʾ��ֵ
uint16_t	HeadPumpInSpeed=0, CTS=0, CTS_Disp=0;//��ͷ������ת��
uint16_t  HeadPumpFSpeed1=0, CTFS1=0, CTFS_Disp1=0;//��ͷǰת��
uint16_t	HeadPumpFSpeed2=0, CTFS2=0, CTFS_Disp2=0;
uint16_t	HeadPumpFSpeed3=0, CTFS3=0, CTFS_Disp3=0;
uint16_t	HeadPumpBSpeed1=0, CTBS1=0, CTBS_Disp1=0;//��ͷ��ת��
uint16_t	HeadPumpBSpeed2=0, CTBS2=0, CTBS_Disp2=0;
uint16_t	HeadPumpBSpeed3=0, CTBS3=0, CTBS_Disp3=0;

float RSF1,RSF2,RSF3,RSB1,RSB2,RSB3;
float RSF10,RSF20,RSF30,RSB10,RSB20,RSB30,CTSIN;

uint8_t Oil_Temperature,Neutral_1,Neutral_2,Grease_Open,Grease_OK,Unload_Cotton,Unload_Warn,Locked,Air_Cleaner,WaterPre_Alarm;
uint8_t ProfileFlag,Reserved_2;/* ���Σ�Ԥ��1����Ԥ��2�����ϱ���*/
uint8_t XRM1Flag,XRM2Flag,XRM3Flag,LHQ1Flag,LHQ2Flag,LHQ3Flag;/*����3·�����š�3·�����*/
uint8_t NeutralBit=0;	//��λ���յ�����־λ


/*---------��ȡ����״̬--------------*/
void ReadAlarmStatus(void)
{

		cp3_parameter.NeutralBit=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0);//�յ�
		cp3_parameter.Median=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1);   //��λ��Ҫ�ж��Ƹ��ڲ�����λ
		cp3_parameter.smooth=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
		cp3_parameter.Locked=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3);
		cp3_parameter.Profile=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
		cp3_parameter.CottonBoxFull=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5); //����Ҫ����
		cp3_parameter.CottonBoxHigh=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
		cp3_parameter.CottonBoxLow=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7);

		cp3_parameter.XRM1Flag=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8);   /*��ȡ����PB9����״̬*/
		cp3_parameter.XRM2Flag=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9);	 /*��ȡ����PB8����״̬*/
		cp3_parameter.XRM3Flag=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10); 
		cp3_parameter.LHQ1Flag=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11); 
		cp3_parameter.LHQ2Flag=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12);		     
		cp3_parameter.LHQ3Flag=HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);     

//		cp3_parameter.HeadAuto = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14);
//		cp3_parameter.HeadCombinne = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);

		if((!(cp3_parameter.XRM1Flag && cp3_parameter.XRM2Flag && cp3_parameter.XRM3Flag 
		&& cp3_parameter.LHQ1Flag && cp3_parameter.LHQ2Flag && cp3_parameter.LHQ3Flag))
		||FJ_Alarm||WaterP_Alarm||OilP_Alarm)   //�����š������������ٶȡ�ˮѹ����
		{ 
			AlarmFlag=0x01;FaultFlag=0x01;WarnFlag=0x00;
		} //���� ��������������
		//else if( FJ_Alarm | WaterP_Alarm | Oil_Temperature | Unload_Cotton | Air_Cleaner | Unload_Warn)//Unload_Warn(CottonBoxFull)
		else 
		{
			if(!cp3_parameter.CottonBoxFull)  //Air_Cleaner��Ҫ��  FJ_Alarm &&
			{																													//�����������
				AlarmFlag=0x01;FaultFlag=0x00;WarnFlag=0x01;
			}	//����

			else
			{
				AlarmFlag=0x00; FaultFlag=0x00;WarnFlag=0x00;
			}  //��������	
		}
	


	 AlarmStatusDate0= (E0_HeartBeat & 0x01)+(WarnFlag<<1)+(FaultFlag<<2)+(Water_Sensor_Break<<3)+(Water_Sensor_Short<<4)+(!cp3_parameter.CottonBoxHigh<<5)+(!cp3_parameter.CottonBoxLow<<6)+(!cp3_parameter.CottonBoxFull<<7);
	 AlarmStatusDate1= !cp3_parameter.XRM1Flag+(!cp3_parameter.XRM2Flag<<1)+(!cp3_parameter.XRM3Flag<<2)+(OilP_Alarm<<5)+(WaterP_Alarm<<6)+(FJ_Alarm<<7);
	 AlarmStatusDate2= !cp3_parameter.LHQ1Flag+(!cp3_parameter.LHQ2Flag<<1)+(!cp3_parameter.LHQ3Flag<<2)+(!cp3_parameter.Median<<7);
	 AlarmStatusDate3= (!cp3_parameter.NeutralBit<<2)+(!cp3_parameter.smooth<<3)+(!cp3_parameter.Locked<<5)+(!cp3_parameter.Profile<<6);

}
											 
///*---------��ȡ�����������--------------*/
//void ReadPulseDatum(void)
//{
//		FJS_Disp=(uint32_t)((FJS*60)/18);	 /*FJS=����ÿ���ӵ���������float��FJS_Disp�ǵ�����ķ��ת��/���ӣ���18���� */
//		if(((1600<=FJS_Disp)&&(FJS_Disp<=3500))||(FJS_Disp>=4500))
//			FJ_Alarm=1;//���,����600��С��3000����״̬����
//		else
//			FJ_Alarm=0;
//		//	printf("\n\r The FJS_Disp is %d\r\n", FJS);

//		CTS_Disp = (U16)((CTS*60)/InGearN);           //Coeff_CTS=0.5
//		// printf("\n\r The Voltage3 is %d.", CTS_Disp);
//		CTSIN = (float)CTS/InGearN;

//		CTFS_Disp1=(U16)((CTFS1*60)/FGearN);
//		RSF10 = (float)CTFS1/FGearN;
//		RSF1 =(float)RSF10/(CTSIN * 0.1326);
//		//printf("\n\r The RSF1 is %f.", RSF1);

//		CTFS_Disp2 = (U16)((CTFS2*60)/FGearN);	
//		RSF20 = (float)CTFS2/FGearN;
//		RSF2 =(float)RSF20/(CTSIN * 0.1326);
//		//printf("\n\r The Voltage2 is %d.", CTFS_Disp2);

//		CTFS_Disp3 = (U16)((CTFS3*60)/FGearN);
//		RSF30 = (float)CTFS3/FGearN;
//		RSF3 =(float)RSF30/(CTSIN * 0.1326);
//		//printf("\n\r The Voltage3 is %d.", CTFS_Disp3);

//		CTBS_Disp1 = (U16)((CTBS1*60)/BGearN);
//		RSB10 = (float)CTBS1/BGearN;
//		RSB1 =(float)RSB10/(CTSIN * 0.1524);
//		//printf("\n\r The Voltage4 is %d.", CTBS_Disp1);

//		CTBS_Disp2 = (U16)((CTBS2*60)/BGearN);
//		RSB20 = (float)CTBS2/BGearN;
//		RSB2 =(float)RSB20/(CTSIN * 0.1524);	
//		//printf("\n\r The Voltage5 is %d.", CTBS_Disp2);

//		CTBS_Disp3 = (U16)((CTBS3*60)/BGearN);
//		RSB30 = (float)CTBS3/BGearN;
//		RSB3 =(float)RSB30/(CTSIN * 0.1524);	
//		//printf("\n\r The Voltage6 is %d.", CTBS_Disp3)

//		//ʹ��ת�ٱ��ж�������Ƿ���������
//		if(RSF1 < LHQ_ratio  || RSB1 < LHQ_ratio)//û�д���������  LHQ1Flag1 Ϊ0
//		cp3_parameter.LHQ1Flag1 = 0;          //û�д���������  LHQ1Flag1 Ϊ0����
//		else cp3_parameter.LHQ1Flag1 = 1;
//		if(RSF2 < LHQ_ratio  || RSB2 < LHQ_ratio)
//		cp3_parameter.LHQ2Flag2 = 0;
//		else cp3_parameter.LHQ2Flag2 = 1;
//		if(RSF3 < LHQ_ratio  || RSB3 < LHQ_ratio)
//		cp3_parameter.LHQ3Flag3 = 0;	
//		else cp3_parameter.LHQ3Flag3 = 1;

//		AlarmStatusDate4= cp3_parameter.LHQ1Flag1+(cp3_parameter.LHQ2Flag2<<1)+(cp3_parameter.LHQ3Flag3<<2);
//}

void GetP_CTINSpeed(void)
{
	uint16_t temp;
	temp=ReadPin_CTIN;
	if(temp==p_speed_variable.P_CTIN_Speed_Value)
		{
		p_speed_variable.P_CTIN_Speed_Counter++;
		if(	p_speed_variable.P_CTIN_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTIN_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTIN_SpeedPulse==0)
					{
					p_speed_variable.CTIN_SpeedCounter++;
					p_speed_variable.CTIN_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTIN_Speed_Value=temp;
		p_speed_variable.P_CTIN_Speed_Counter=0;
		p_speed_variable.CTIN_SpeedPulse=0;
	}
		
}

void GetP_CTF1Speed(void)
{
	uint16_t temp;
	temp=ReadPin_CTF1;
	if(temp==p_speed_variable.P_CTF1_Speed_Value)
		{
		p_speed_variable.P_CTF1_Speed_Counter++;
		if(	p_speed_variable.P_CTF1_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTF1_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTF1_SpeedPulse==0)
					{
					p_speed_variable.CTF1_SpeedCounter++;
					p_speed_variable.CTF1_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTF1_Speed_Value=temp;
		p_speed_variable.P_CTF1_Speed_Counter=0;
		p_speed_variable.CTF1_SpeedPulse=0;
	}

}

void GetP_CTB1Speed(void)
{
	uint16_t temp;
	temp=ReadPin_CTB1;
	if(temp==p_speed_variable.P_CTB1_Speed_Value)
		{
		p_speed_variable.P_CTB1_Speed_Counter++;
		if(	p_speed_variable.P_CTB1_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTB1_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTB1_SpeedPulse==0)
					{
					p_speed_variable.CTB1_SpeedCounter++;
					p_speed_variable.CTB1_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTB1_Speed_Value=temp;
		p_speed_variable.P_CTB1_Speed_Counter=0;
		p_speed_variable.CTB1_SpeedPulse=0;
	}
	
}
void GetP_CTF2Speed(void)
{
	uint16_t temp;
	temp=ReadPin_CTF2;
	if(temp==p_speed_variable.P_CTF2_Speed_Value)
		{
		p_speed_variable.P_CTF2_Speed_Counter++;
		if(	p_speed_variable.P_CTF2_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTF2_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTF2_SpeedPulse==0)
					{
					p_speed_variable.CTF2_SpeedCounter++;
					p_speed_variable.CTF2_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTF2_Speed_Value=temp;
		p_speed_variable.P_CTF2_Speed_Counter=0;
		p_speed_variable.CTF2_SpeedPulse=0;
	}
	
}

void GetP_CTB2Speed(void)
{
	uint16_t temp;
	temp=ReadPin_CTB2;
	if(temp==p_speed_variable.P_CTB2_Speed_Value)
		{
		p_speed_variable.P_CTB2_Speed_Counter++;
		if(	p_speed_variable.P_CTB2_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTB2_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTB2_SpeedPulse==0)
					{
					p_speed_variable.CTB2_SpeedCounter++;
					p_speed_variable.CTB2_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTB2_Speed_Value=temp;
		p_speed_variable.P_CTB2_Speed_Counter=0;
		p_speed_variable.CTB2_SpeedPulse=0;
	}
}

void GetP_CTF3Speed(void)
{
	uint16_t temp;
	temp=ReadPin_CTF3;
	if(temp==p_speed_variable.P_CTF3_Speed_Value)
		{
		p_speed_variable.P_CTF3_Speed_Counter++;
		if(	p_speed_variable.P_CTF3_Speed_Counter>=2)//����2��80us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTF3_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTF3_SpeedPulse==0)
					{
					p_speed_variable.CTF3_SpeedCounter++;
					p_speed_variable.CTF3_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTF3_Speed_Value=temp;
		p_speed_variable.P_CTF3_Speed_Counter=0;
		p_speed_variable.CTF3_SpeedPulse=0;
	}
		
}

void GetP_CTB3Speed(void)
{
	uint16_t temp;
	temp=ReadPin_CTB3;
	if(temp==p_speed_variable.P_CTB3_Speed_Value)
		{
		p_speed_variable.P_CTB3_Speed_Counter++;
		if(	p_speed_variable.P_CTB3_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_CTB3_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.CTB3_SpeedPulse==0)
					{
					p_speed_variable.CTB3_SpeedCounter++;
					p_speed_variable.CTB3_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_CTB3_Speed_Value=temp;
		p_speed_variable.P_CTB3_Speed_Counter=0;
		p_speed_variable.CTB3_SpeedPulse=0;
	}

}

void GetP_FanSpeed(void)
{
	uint16_t temp;
	temp=ReadPin_FanSpeed;
	if(temp==p_speed_variable.P_Fan_Speed_Value)
		{
		p_speed_variable.P_Fan_Speed_Counter++;
		if(	p_speed_variable.P_Fan_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_Fan_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.Fan_SpeedPulse==0)
					{
					p_speed_variable.Fan_SpeedCounter++;
					p_speed_variable.Fan_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_Fan_Speed_Value=temp;
		p_speed_variable.P_Fan_Speed_Counter=0;
		p_speed_variable.Fan_SpeedPulse=0;
	}
		
}

void GetP_WalkSpeed(void)
{
	uint16_t temp;
	temp=ReadPin_WalkSpeed;
	if(temp==p_speed_variable.P_Walk_Speed_Value)
		{
		p_speed_variable.P_Walk_Speed_Counter++;
		if(	p_speed_variable.P_Walk_Speed_Counter>=2)//����5��50us�ɼ�����ͬ��ֵ����Ϊ����Чֵ
			{			
				p_speed_variable.P_Walk_Speed_Counter=0;
			if(temp==1)//�ڵ͵�ƽ���������ڸߵ�ƽ����1��
				{
				if(p_speed_variable.Walk_SpeedPulse==0)
					{
					p_speed_variable.Walk_SpeedCounter++;
					p_speed_variable.Walk_SpeedPulse=1;
					}
				}
			}
		}
	else
	{
		p_speed_variable.P_Walk_Speed_Value=temp;
		p_speed_variable.P_Walk_Speed_Counter=0;
		p_speed_variable.Walk_SpeedPulse=0;
	}
		
}
