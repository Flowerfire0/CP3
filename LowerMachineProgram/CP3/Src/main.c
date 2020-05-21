/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "delay.h"
#include "24CXX.h"
#include "DataCollection.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void InitEEPROM(void)
{
//	AT24CXX_WriteLenByte(0x00,BackTransmissionRatio_Can,2);
//	AT24CXX_WriteLenByte(0x04,FrontTransmissionRatio_Can,2);		
//	AT24CXX_WriteLenByte(0x08,Coeff_Land_Can, 2);
//	AT24CXX_WriteLenByte(0x0C,LHQ_ratio_Can, 2);
//	AT24CXX_WriteLenByte(0x10,XZ_length_ratio_Can, 2);
//	AT24CXX_WriteLenByte(0x14,XZZX_Can, 2);
//	AT24CXX_WriteLenByte(0x18,MachineID, 2);
//	AT24CXX_WriteLenByte(0x1C,Coeff_head_Locked_Can, 2);
//	AT24CXX_WriteLenByte(0x20,InGearN, 2);
//	AT24CXX_WriteLenByte(0x24,FGearN, 2);
//	AT24CXX_WriteLenByte(0x28,BGearN, 2);
//	AT24CXX_WriteLenByte(0x2C,CTS_SET1, 2);
//	AT24CXX_WriteLenByte(0x30,CTS_SET2, 2);
//	AT24CXX_WriteLenByte(0x34,CTS_SET3, 2);
//	AT24CXX_WriteLenByte(0x38,CTS_SET4, 2);
//	AT24CXX_WriteLenByte(0x3C,CTS_SET5, 2);
//	AT24CXX_WriteLenByte(0x40,CTS_SET6, 2);
//	AT24CXX_WriteLenByte(0x44,OilTempAlarm, 2);
//	AT24CXX_WriteLenByte(0x80, HarvestedLand, 4);
//	AT24CXX_WriteLenByte(0x84, TotalHarvestedLand, 4);	
}
void ReadFromEEPROM(void)
{
//	BackTransmissionRatio_Can = AT24CXX_ReadLenByte(0x00, 2);
//	FrontTransmissionRatio_Can = AT24CXX_ReadLenByte(0x04, 2);
//	Coeff_Land_Can = AT24CXX_ReadLenByte(0x08, 2);
//	LHQ_ratio_Can = AT24CXX_ReadLenByte(0x0C, 2);
//	XZ_length_ratio_Can = AT24CXX_ReadLenByte(0x10, 2);
//	XZZX_Can = AT24CXX_ReadLenByte(0x14, 2);
//	MachineID = AT24CXX_ReadLenByte(0x18, 2);
//	Coeff_head_Locked_Can = AT24CXX_ReadLenByte(0x1C, 2);
//	InGearN = AT24CXX_ReadLenByte(0x20, 2);
//	FGearN = AT24CXX_ReadLenByte(0x24, 2);
//	BGearN = AT24CXX_ReadLenByte(0x28, 2);
//	CTS_SET1 = AT24CXX_ReadLenByte(0x2C, 2);
//	CTS_SET2 = AT24CXX_ReadLenByte(0x30, 2);
//	CTS_SET3 = AT24CXX_ReadLenByte(0x34, 2);
//	CTS_SET4 = AT24CXX_ReadLenByte(0x38, 2);
//	CTS_SET5 = AT24CXX_ReadLenByte(0x3C, 2);
//	CTS_SET6 = AT24CXX_ReadLenByte(0x40, 2);
//	OilTempAlarm = AT24CXX_ReadLenByte(0x44, 2);
//	HarvestedLand = AT24CXX_ReadLenByte(0x80, 4);
//	TotalHarvestedLand = AT24CXX_ReadLenByte(0x84, 4);	
}
void EEPROM_DATA_Init(void)
{
	uint8_t temp;
	uint8_t dat=0x55;
	uint8_t ret;
	
	ret=AT24CXX_Check();
	if(ret==0)
		printf("AT24CXX_Check  success!\r\n");
	else
		printf("AT24CXX_Check  fail!\r\n");
	
	At24c02_Read_Byte(0xF0,&temp);	
printf("temp=%x\r\n",temp);	
	if(temp!=0X55)
	{
		printf("New board, Init EEPROM!\r\n");
		At24c02_Write_Byte(0xF0,&dat);
		osDelay(10);
		At24c02_Read_Byte(0xF0,&temp);	
printf("temp=%x\r\n",temp);	
		InitEEPROM();
	}		
//	else
//	{
//		printf("It is not a New board!\r\n");
//	}
	
//	ReadFromEEPROM();
		dat=0x12;
		At24c02_Write_Byte(0x14,&dat);
	osDelay(1);
	At24c02_Read_Byte(0x14,&temp);
		BackTransmissionRatio_Can = temp;
	printf("BackTransmissionRatio_Can=%d,temp=%d\r\n",BackTransmissionRatio_Can,temp);
		//	if(eeprom_flag != 0x55)
//	{
//		eeprom_flag = 0x55;
//		AT24CXX_WriteLenByte(0xF0, eeprom_flag, 4);
//		InitEEPROM();
//	}
//	else
//	{
//		ReadFromEEPROM();
//	}
}
void InitParameter(void)
{
	cp3_parameter.Fan_speed = 0;
	cp3_parameter.Fan_speedB = 0;	
	cp3_parameter.Forward_speed = 0;
	cp3_parameter.HeadPumpInSpeed = 0;
	cp3_parameter.HeadPumpBSpeed1 = 0;
	cp3_parameter.HeadPumpBSpeed2 = 0;
	cp3_parameter.HeadPumpBSpeed3 = 0;
	cp3_parameter.HeadPumpFSpeed1 = 0;
	cp3_parameter.HeadPumpFSpeed2 = 0;
	cp3_parameter.HeadPumpFSpeed3 = 0;
	cp3_parameter.ControllerHandle = 0;
	cp3_parameter.HeadPump = 0;
	cp3_parameter.TravelPump = 0;
	cp3_parameter.Water_Pres = 0;
	cp3_parameter.Median_flag=1;
	cp3_parameter.LHQ1Flag = 1;
	cp3_parameter.LHQ2Flag = 1;
	cp3_parameter.LHQ3Flag = 1;
	cp3_parameter.LHQ1Flag1 = 1;
	cp3_parameter.LHQ2Flag2 = 1;
	cp3_parameter.LHQ3Flag3 = 1;
	cp3_parameter.HeadAuto = 1;
	cp3_parameter.HeadCombinne = 1;
	p_speed_variable.P_CTIN_Speed_Counter=0;              
	p_speed_variable.P_CTIN_Speed_Value=0;								
	p_speed_variable.CTIN_SpeedCounter=0;                 
	p_speed_variable.CTIN_SpeedPulse=0;                   

	p_speed_variable.P_CTF1_Speed_Counter=0;              
	p_speed_variable.P_CTF1_Speed_Value=0;		
	p_speed_variable.CTF1_SpeedCounter=0;                 
	p_speed_variable.CTF1_SpeedPulse=0; 

	p_speed_variable.P_CTB1_Speed_Counter=0;              
	p_speed_variable.P_CTB1_Speed_Value=0;	
	p_speed_variable.CTB1_SpeedCounter=0;                 
	p_speed_variable.CTB1_SpeedPulse=0; 

	p_speed_variable.P_CTF2_Speed_Counter=0;              
	p_speed_variable.P_CTF2_Speed_Value=0;	
	p_speed_variable.CTF2_SpeedCounter=0;                 
	p_speed_variable.CTF2_SpeedPulse=0; 

	p_speed_variable.P_CTB2_Speed_Counter=0;              
	p_speed_variable.P_CTB2_Speed_Value=0;		
	p_speed_variable.CTB2_SpeedCounter=0;                 
	p_speed_variable.CTB2_SpeedPulse=0; 

	p_speed_variable.P_CTF3_Speed_Counter=0;              
	p_speed_variable.P_CTF3_Speed_Value=0;	
	p_speed_variable.CTF3_SpeedCounter=0;                 
	p_speed_variable.CTF3_SpeedPulse=0; 

	p_speed_variable.P_CTB3_Speed_Counter=0;              
	p_speed_variable.P_CTB3_Speed_Value=0;	
	p_speed_variable.CTB3_SpeedCounter=0;                 
	p_speed_variable.CTB3_SpeedPulse=0; 	
	
	p_speed_variable.P_Fan_Speed_Counter=0;              
	p_speed_variable.P_Fan_Speed_Value=0;	
	p_speed_variable.Fan_SpeedCounter=0;                 
	p_speed_variable.Fan_SpeedPulse=0; 

	p_speed_variable.P_Walk_Speed_Counter=0;              
	p_speed_variable.P_Walk_Speed_Value=0;	
	p_speed_variable.Walk_SpeedCounter=0;                 
	p_speed_variable.Walk_SpeedPulse=0;
		
  AlarmStatusDate0=0x00; AlarmStatusDate1=0x00; 
  AlarmStatusDate2=0x00; AlarmStatusDate3=0x00;
	
  WarnFlag=0x00;
  Water_Sensor_Break=0x00;
  Water_Sensor_Short=0x00;	 
	
	Water_Sensor_Break=0x00;
  Water_Sensor_Short=0x00;
	
	MedianConsistency_Con(1);
	
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_IWDG_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	printf("Hello world!\r\n");
	CAN_User_Init( &hcan1 );
//	I2C1_Init();
//	delay_init();
	InitParameter();
	
	EEPROM_DATA_Init();


	
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&AD_Value, MAX_AD_NUM);	
	HAL_TIM_Base_Start_IT(&htim4);	
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Systick interrupt time 
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
	if (htim->Instance == TIM4){//50us
		
		GetP_CTINSpeed();	
		GetP_CTF1Speed();			
		GetP_CTB1Speed();	
		GetP_CTF2Speed();			
		GetP_CTB2Speed();	
		GetP_CTF3Speed();	
		GetP_CTB3Speed();	
		GetP_FanSpeed();
		GetP_WalkSpeed();
	}
	
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
