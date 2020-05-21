/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
uint8_t E0_HeartBeat=0;

//static uint32_t cnt_1s = 0;
/* USER CODE END 0 */

TIM_HandleTypeDef htim4;

/* TIM4 init function */
void MX_TIM4_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 72-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 50-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* TIM4 interrupt Init */
    HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /* TIM4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
// 
//	if (htim->Instance == TIM5){//100ms
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);	
//		if (++cnt_1s>=10)   //1S定时
//		{ 
//			CTS=p_speed_variable.CTIN_SpeedCounter;
//			p_speed_variable.CTIN_SpeedCounter=0;		
//			CTFS1=p_speed_variable.CTF1_SpeedCounter;
//			p_speed_variable.CTF1_SpeedCounter=0;
//			CTBS1=p_speed_variable.CTB1_SpeedCounter;
//			p_speed_variable.CTB1_SpeedCounter=0;
//			CTFS2=p_speed_variable.CTF2_SpeedCounter;
//			p_speed_variable.CTF2_SpeedCounter=0;
//			CTBS2=p_speed_variable.CTB2_SpeedCounter;
//			p_speed_variable.CTB2_SpeedCounter=0;
//			CTFS3=p_speed_variable.CTF3_SpeedCounter;
//			p_speed_variable.CTF3_SpeedCounter=0;
//			CTBS3=p_speed_variable.CTB3_SpeedCounter;
//			p_speed_variable.CTB3_SpeedCounter=0;	
//			FJS = p_speed_variable.Fan_SpeedCounter;
//			p_speed_variable.Fan_SpeedCounter=0;	
//			XZS = p_speed_variable.Walk_SpeedCounter;
//			p_speed_variable.Walk_SpeedCounter=0;	

//			if(CTS >20 || cp3_parameter.Profile == 0)							 //仿形有效
//			{	
//				DeltaLand=Coeff_Land*XZS*0.00001424*2.28;					   //计算每秒钟的增量（亩）	0.0015*(3.1416*1.635*11/85/70)
//				HarvestedLand=HarvestedLand+DeltaLand;		    //小计收获面积累加（按清零后的累加）
//				TotalHarvestedLand=TotalHarvestedLand+DeltaLand; //总计收获面积累加
//				DeltaLand=0.0;
//			}
//			E0_HeartBeat= ~E0_HeartBeat; //心跳			
//			cnt_1s=0;
//		}
//		if (++cnt_1m>=600)//1分钟定时到
//		{ 
////			WriteLandToEEPROM();
//			WriteLandFlag = 1;
//			TotalHarvestedLand_old = TotalHarvestedLand;
//			cnt_1m=0;
//		}	 
//	}
	
//	if (htim->Instance == TIM3){//1ms
//		sum_Water_Pres+=AD_Value[0];
//		sum_ControllerHandle+=AD_Value[1];
//		sum_TravelPump+=AD_Value[2];
//		sum_HeadPump+=AD_Value[3];
//		if (++cnt_control>=5)   //5mS定时
//		{ 			
//		  cnt_control=0;
//			
//			sum_Water_Pres=0;
//			sum_ControllerHandle=0;
//			sum_TravelPump=0;
//			sum_HeadPump=0;
//		}		
//	}

//	if (htim->Instance == TIM1) {
//	HAL_IncTick();
//	}
//	
//	if (htim->Instance == TIM4){//50us
//		
//		GetP_CTINSpeed();	
//		GetP_CTF1Speed();			
//		GetP_CTB1Speed();	
//		GetP_CTF2Speed();			
//		GetP_CTB2Speed();	
//		GetP_CTF3Speed();	
//		GetP_CTB3Speed();	
//		GetP_FanSpeed();
//		GetP_WalkSpeed();
//	}
//	
//}



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
