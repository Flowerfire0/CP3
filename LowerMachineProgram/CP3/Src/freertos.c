/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "user_config.h"
#include "can.h"
#include "iwdg.h"
#include "DataCollection.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Task_CANHandle;
osThreadId Task_FeedDogHandle;
osThreadId Task_DataColHandle;
osThreadId Task_AreaCalHandle;
osThreadId Tack_WalkSpeedCHandle;
osThreadId Task_HeadSpeedCHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void CanTask(void const * argument);
void FeedDogTask(void const * argument);
void DataCollectionTask(void const * argument);
void AreaCalTask(void const * argument);
void WalkSpeedControlTask(void const * argument);
void HeadSpeedControlTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task_CAN */
  osThreadDef(Task_CAN, CanTask, osPriorityRealtime, 0, 128);
  Task_CANHandle = osThreadCreate(osThread(Task_CAN), NULL);

  /* definition and creation of Task_FeedDog */
  osThreadDef(Task_FeedDog, FeedDogTask, osPriorityNormal, 0, 64);
  Task_FeedDogHandle = osThreadCreate(osThread(Task_FeedDog), NULL);

  /* definition and creation of Task_DataCol */
  osThreadDef(Task_DataCol, DataCollectionTask, osPriorityRealtime, 0, 128);
  Task_DataColHandle = osThreadCreate(osThread(Task_DataCol), NULL);

  /* definition and creation of Task_AreaCal */
  osThreadDef(Task_AreaCal, AreaCalTask, osPriorityRealtime, 0, 128);
  Task_AreaCalHandle = osThreadCreate(osThread(Task_AreaCal), NULL);

  /* definition and creation of Tack_WalkSpeedC */
  osThreadDef(Tack_WalkSpeedC, WalkSpeedControlTask, osPriorityRealtime, 0, 128);
  Tack_WalkSpeedCHandle = osThreadCreate(osThread(Tack_WalkSpeedC), NULL);

  /* definition and creation of Task_HeadSpeedC */
  osThreadDef(Task_HeadSpeedC, HeadSpeedControlTask, osPriorityRealtime, 0, 128);
  Task_HeadSpeedCHandle = osThreadCreate(osThread(Task_HeadSpeedC), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
uint8_t eeprom_write_flag2=0;
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    
    
    
    
    
    
    

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		if(eeprom_write_flag2==1){
//				AT24CXX_WriteLenByte(0x00,BackTransmissionRatio_Can,2);
//				AT24CXX_WriteLenByte(0x04,FrontTransmissionRatio_Can,2);
//				AT24CXX_WriteLenByte(0x0C,LHQ_ratio_Can,2);
//				AT24CXX_WriteLenByte(0x44,OilTempAlarm,2);
			eeprom_write_flag2=0;
		}
		osDelay(10);		
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_CanTask */
/**
* @brief Function implementing the Task_CAN thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CanTask */
void CanTask(void const * argument)
{
  /* USER CODE BEGIN CanTask */
	uint16_t Can_cnt = 0;
  /* Infinite loop */
  for(;;)
  {
		Can_cnt++;
		osDelay(100);
		CAN_Transmit1();
		osDelay(10);
		CAN_Transmit2();
		osDelay(10);
		CAN_Transmit3();
		osDelay(10);
		CAN_Transmit4();
		osDelay(10);
		CAN_Transmit5();
		osDelay(10);
		CAN_Transmit6();
		osDelay(50);
		if(Can_cnt > 5)
		{
			E0_HeartBeat= ~E0_HeartBeat; //1s����
			Can_cnt = 0;
		}
  }
  /* USER CODE END CanTask */
}

/* USER CODE BEGIN Header_FeedDogTask */
/**
* @brief Function implementing the Task_FeedDog thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FeedDogTask */
void FeedDogTask(void const * argument)
{
  /* USER CODE BEGIN FeedDogTask */
  /* Infinite loop */
  for(;;)
  {
		HAL_IWDG_Refresh(&hiwdg);	
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);	
    osDelay(500);		
  }
  /* USER CODE END FeedDogTask */
}

/* USER CODE BEGIN Header_DataCollectionTask */
/**
* @brief Function implementing the Task_DataCol thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataCollectionTask */
void DataCollectionTask(void const * argument)
{
  /* USER CODE BEGIN DataCollectionTask */
	uint16_t DataCollectionTask_cnt = 0;
	float BackTransmissionRatio, FrontTransmissionRatio;
  /* Infinite loop */
  for(;;)
  {
		DataCollectionTask_cnt++;
		sum_Water_Pres+=AD_Value[0];
		sum_OilTemp+=AD_Value[1];
		sum_ControllerHandle+=AD_Value[2];
		sum_TravelPump+=AD_Value[3];
		sum_HeadPump+=AD_Value[4];
		if(DataCollectionTask_cnt%5==0)//50ms  �ɼ�ģ������������
		{
			ReadAlarmStatus();
			ReadADDatum();
			sum_Water_Pres=0; 
			sum_OilTemp=0;
			sum_ControllerHandle=0;
			sum_TravelPump=0;
			sum_HeadPump=0;   
		}
		
		if(DataCollectionTask_cnt%20==0)//200ms  ������ת�٣���ͷת�ٲ�����ͷת�ٱ����ж�
		{
			FJS_Disp=(uint32_t)((p_speed_variable.Fan_SpeedCounter*60)/18);	 /*FJS=����ÿ���ӵ���������float��FJS_Disp�ǵ�����ķ��ת��/���ӣ���18���� */
			p_speed_variable.Fan_SpeedCounter=0;
			if(((1600<=FJS_Disp)&&(FJS_Disp<=3500))||(FJS_Disp>=4500))
				FJ_Alarm=1;//���,����600��С��3000����״̬����
			else
				FJ_Alarm=0;			
			
			LHQ_ratio = 0.01*LHQ_ratio_Can;
			CTS_Disp = (uint16_t)((p_speed_variable.CTIN_SpeedCounter*60)/InGearN); //rpm
			p_speed_variable.CTIN_SpeedCounter=0;  

			CTFS_Disp1=(uint16_t)((p_speed_variable.CTF1_SpeedCounter*60)/FGearN);//rpm
			p_speed_variable.CTF1_SpeedCounter=0;
			CTBS_Disp1=(uint16_t)((p_speed_variable.CTB1_SpeedCounter*60)/BGearN);//rpm
			p_speed_variable.CTB1_SpeedCounter=0;
			
			BackTransmissionRatio = 0.0001 * BackTransmissionRatio_Can;
			FrontTransmissionRatio = 0.0001 * FrontTransmissionRatio_Can;
			if((CTS_Disp>500)&&((CTFS_Disp1/(CTS_Disp* FrontTransmissionRatio))< LHQ_ratio  || (CTBS_Disp1/(CTS_Disp * BackTransmissionRatio))< LHQ_ratio ))//��ͷת�ٴ���500rpm,���б����ж�
				cp3_parameter.LHQ1Flag1 = 0;          //û�д���������  LHQ1Flag1 Ϊ0����
			else 
				cp3_parameter.LHQ1Flag1 = 1; //����

			CTFS_Disp2=(uint16_t)((p_speed_variable.CTF2_SpeedCounter*60)/FGearN);//rpm
			p_speed_variable.CTF2_SpeedCounter=0;
			CTBS_Disp2=(uint16_t)((p_speed_variable.CTB2_SpeedCounter*60)/BGearN);//rpm
			p_speed_variable.CTB2_SpeedCounter=0;

			if((CTS_Disp>500)&&((CTFS_Disp2/(CTS_Disp* FrontTransmissionRatio))< LHQ_ratio  || (CTBS_Disp2/(CTS_Disp * BackTransmissionRatio))< LHQ_ratio ))//��ͷת�ٴ���500rpm,���б����ж�
				cp3_parameter.LHQ2Flag2 = 0;          //û�д���������  LHQ2Flag2 Ϊ0����
			else 
				cp3_parameter.LHQ2Flag2 = 1; //����

			CTFS_Disp3=(uint16_t)((p_speed_variable.CTF3_SpeedCounter*60)/FGearN);//rpm
			p_speed_variable.CTF3_SpeedCounter=0;
			CTBS_Disp3=(uint16_t)((p_speed_variable.CTB3_SpeedCounter*60)/BGearN);//rpm
			p_speed_variable.CTB3_SpeedCounter=0;

			if((CTS_Disp>500)&&((CTFS_Disp3/(CTS_Disp* FrontTransmissionRatio))< LHQ_ratio  || (CTBS_Disp3/(CTS_Disp * BackTransmissionRatio))< LHQ_ratio ))//��ͷת�ٴ���500rpm,���б����ж�
				cp3_parameter.LHQ3Flag3 = 0;          //û�д���������  LHQ3Flag3 Ϊ0����
			else 
				cp3_parameter.LHQ3Flag3 = 1; //����	 
			
			AlarmStatusDate4= cp3_parameter.LHQ1Flag1+(cp3_parameter.LHQ2Flag2<<1)+(cp3_parameter.LHQ3Flag3<<2);
		}
		osDelay(10);//10ms

		if(DataCollectionTask_cnt>=20)
			DataCollectionTask_cnt=0;

  }
  /* USER CODE END DataCollectionTask */
}

/* USER CODE BEGIN Header_AreaCalTask */
/**
* @brief Function implementing the Task_AreaCal thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AreaCalTask */
void AreaCalTask(void const * argument)
{
  /* USER CODE BEGIN AreaCalTask */
	uint16_t AreaCalTask_cnt1 = 0;
	uint16_t speed_cnt = 0;
	float DeltaLand=0.0;
	float XZ_speed = 0.0;
  /* Infinite loop */
  for(;;)
  {
		AreaCalTask_cnt1++;

		speed_cnt=p_speed_variable.Walk_SpeedCounter;//1s�������
		p_speed_variable.Walk_SpeedCounter=0;	
		//���������ٶ�
		XZ_speed=(float)(speed_cnt*0.034185888);
		XZS_Disp = (uint16_t)(10*XZ_speed);
		/*(uint32_t)(60.0*60.0*3.1416*1.635*Coeff_Walking_Speed*XZS*11.0/85.0/70.0/100.0);	 XZS=����ÿ���ӵ���������float��XZS_Disp 	//0.34168   *0.34185
															�����䴦ȡ��XZS=����ϵ��*(������/������/����)*60*60*����ֱ��*pi/1000,
															3�л�����70��ֱ��1.635m��������85:11���˴�Ӧ����0.034168��
															���ǵ���ʾ1λС���㣬Ҫ����10 */				
		if(XZ_speed < 0.1 || flag_Reverse == 1)
			CTS_set = 0;
		else if(XZ_speed >= 0.1 && XZ_speed < 2)
			CTS_set = CTS_SET1;
		else if(XZ_speed >= 2 && XZ_speed < 3)
			CTS_set = CTS_SET2;
		else if(XZ_speed >= 3 && XZ_speed < 4)
			CTS_set = CTS_SET3;
		else if(XZ_speed >= 4 && XZ_speed < 5)
			CTS_set = CTS_SET4;
		else if(XZ_speed >= 5 && XZ_speed < 6)
			CTS_set = CTS_SET5;			
		else if(XZ_speed >= 6)
			CTS_set = CTS_SET6;			
		
		if(CTS_Disp > 500 || cp3_parameter.Profile == 0)							 //������Ч���߲�ͷת�ٴ���500rpm
		{				
			//�������
			Coeff_Land = 0.01*Coeff_Land_Can;
			DeltaLand = Coeff_Land*speed_cnt*0.00001424*2.28;					   //����ÿ���ӵ�������Ķ��	0.0015*(3.1416*1.635*11/85/70)
			HarvestedLand += (u32)(DeltaLand*100);		    //С���ջ�����ۼӣ����������ۼӣ�
			TotalHarvestedLand += (u32)(DeltaLand*100); //�ܼ��ջ�����ۼ�
			DeltaLand=0.0;
			AreaCalTask_cnt1 = 0;
		}
		if(AreaCalTask_cnt1 > 30 && TotalHarvestedLand - TotalHarvestedLand_old > 0)//30sһ��
		{
			AreaCalTask_cnt1 = 0;
			TotalHarvestedLand_old = TotalHarvestedLand;
//			AT24CXX_WriteLenByte(0x80,HarvestedLand,4);	
//			AT24CXX_WriteLenByte(0x84,TotalHarvestedLand,4);				
		}		
    osDelay(1000);
  }
  /* USER CODE END AreaCalTask */
}

/* USER CODE BEGIN Header_WalkSpeedControlTask */
/**
* @brief Function implementing the Tack_WalkSpeedC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WalkSpeedControlTask */
void WalkSpeedControlTask(void const * argument)
{
  /* USER CODE BEGIN WalkSpeedControlTask */
	uint16_t WalkSpeedControlTask_cnt = 0;
  /* Infinite loop */
  for(;;)
  {
		if(Flag_Median == 1 && cp3_parameter.Median != 1)
		{
			WalkSpeedControlTask_cnt++;
			if(WalkSpeedControlTask_cnt > 100)
			{
				MedianConsistency_Con(0);
			}
		}
		else
		{
			MedianConsistency_Con(1);	
			WalkSpeedControlTask_cnt = 0;
		}

		TravelControl();
    osDelay(50);
  }
  /* USER CODE END WalkSpeedControlTask */
}

/* USER CODE BEGIN Header_HeadSpeedControlTask */
/**
* @brief Function implementing the Task_HeadSpeedC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HeadSpeedControlTask */
void HeadSpeedControlTask(void const * argument)
{
  /* USER CODE BEGIN HeadSpeedControlTask */
  /* Infinite loop */
  for(;;)
  {
		HeadControl();
    osDelay(200);
  }
  /* USER CODE END HeadSpeedControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
