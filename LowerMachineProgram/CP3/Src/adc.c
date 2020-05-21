/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
uint16_t Water_Pres_Avr = 0;
uint16_t OilTemp_Avr = 0;
uint16_t ControllerHandle_Avr = 0;
uint16_t TravelPump_Avr = 0;
uint16_t HeadPump_Avr = 0;

uint32_t sum_Water_Pres=0;
uint32_t sum_OilTemp=0;
uint32_t sum_ControllerHandle=0;
uint32_t sum_TravelPump=0;
uint32_t sum_HeadPump=0;


uint8_t Water_Sensor_Break=0,Water_Sensor_Short=0;	 //水压传感器断路，短路

__IO  uint16_t AD_Value[MAX_AD_NUM];
uint16_t OilTempAlarm = 80;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 5;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC4     ------> ADC1_IN14
    PC5     ------> ADC1_IN15 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4 
                          |GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC4     ------> ADC1_IN14
    PC5     ------> ADC1_IN15 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4 
                          |GPIO_PIN_5);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
uint32_t OilTemp_R;
float OilTemp_V;
float ftemp1,ftemp2;
void ReadADDatum(void)
{
	int temp;
	int oiltemp;

		Water_Pres_Avr=sum_Water_Pres/5+55;	//读取水压AD值，加0.045V偏差
		if(Water_Pres_Avr > 640) //640约为18Kpa，因为4ma(AD值745)对应0Mpa，给传感器留一定的跳动余量，压力小于18Kpa显示0值，	  
			cp3_parameter.Water_Pres = 0.4196*Water_Pres_Avr-250.0; 	   //y=k(x-x1)+y1,k=(y2-y1)/(x2-x1)   pa=(v-595)*(1000-0)/(2978-595)+0; //4-20ma  120Ω 转化为电压对应0.48-2.4V =595-2978	 单位Kpa
		else
			cp3_parameter.Water_Pres=0;

		if(Water_Pres_Avr<300){Water_Sensor_Break=0x01;} else {Water_Sensor_Break=0x00;}//375
		if(Water_Pres_Avr>3050){Water_Sensor_Short=0x01;} else {Water_Sensor_Short=0x00;}	// 水压1mpa对应2978	，稍微大一点取3050

		if((cp3_parameter.Water_Pres<300 &&  cp3_parameter.Water_Pres>50)|| cp3_parameter.Water_Pres<30)     //cp3_parameter.Water_Pres<1100kpa
			WaterP_Alarm=0;	  //水压正常
		else
			WaterP_Alarm=1;

		OilTemp_V = (sum_OilTemp/5.0)*3.3/4096+0.040;
//		U16 temp = OilTemp_Avr*0.0806;//100*3.3/4096=0.0806
//		if(temp >= 46 && temp <69)
//			cp3_parameter.Oil_Temp = -87 * temp + 160;//100-120℃
//		else if(temp >= 69 && temp <102)
//			cp3_parameter.Oil_Temp = -60.6 * temp + 141.8;//80-100
//		else if(temp >= 102 && temp < 143)
//			cp3_parameter.Oil_Temp = -48.8 * temp + 130;//60-80
//		else if(temp >= 143 && temp <= 185)
//			cp3_parameter.Oil_Temp = -47.6 * temp + 128;//40-60		
//		else
//			cp3_parameter.Oil_Temp = 0;
		//res = 0.86*OilTemp_Avr/(2.5-0.000806*OilTemp_Avr);//2.5*x/(100+x) = OilTemp_Avr*3.3/4096保留一位小数 X10
		if(OilTemp_V>2.35)
		{
			cp3_parameter.Oil_Temp =0;//断路
		}
		else
		{
			ftemp1=2.515-OilTemp_V;
			ftemp2=100.0*OilTemp_V*10.0;
			OilTemp_R=(uint32_t)(ftemp2/ftemp1);
			//uint16_t temp = 0.967*OilTemp_Avr/(2.5-0.000806*OilTemp_Avr);//2.5*x/(120+x) = OilTemp_Avr*3.3/4096保留一位小数 X10
			temp=OilTemp_R;
			if(temp < 385)
				cp3_parameter.Oil_Temp = (uint32_t)(-0.127*temp+148.8);//100-120℃
			else if(temp >= 385 && temp < 691)
				cp3_parameter.Oil_Temp = (uint32_t)(-0.065*temp+124.9);//80-100℃
			else if(temp >= 691 && temp < 1340)
				cp3_parameter.Oil_Temp = (uint32_t)(-0.031*temp+101.5);//60-80℃
			else if(temp >= 1340)
			{
				oiltemp = -0.013*temp+77.3;//40℃以下
				if(oiltemp< 0)	oiltemp=0;
				cp3_parameter.Oil_Temp =(uint32_t)oiltemp;//40℃以下
			}
		}
		if(cp3_parameter.Oil_Temp <= OilTempAlarm)
			OilP_Alarm=0;	  //油温正常
		else
			OilP_Alarm=1;
		
		ControllerHandle_Avr = sum_ControllerHandle/5;//Average_Filter(1); 
		cp3_parameter.ControllerHandle = 0.161*ControllerHandle_Avr;// 2 * 100 * 3.3/4096;//放大100倍，电路上缩小2倍，这里放大2倍
		//printf("\n\r The Voltage1 is %ld.", cp3_parameter.ControllerHandle);


		TravelPump_Avr = sum_TravelPump/5;//Average_Filter(2);
		//printf("\n\r The Voltage2 is %d.\r\n", TravelPump_Avr);
		cp3_parameter.TravelPump = 0.161*TravelPump_Avr;
		//printf("\n\r The TravelPump is %ld.", cp3_parameter.TravelPump);

		HeadPump_Avr = sum_HeadPump/5;//Average_Filter(3); 
		//printf("\n\r The Voltage0 is %d.\r\n", HeadPump_Avr);
		cp3_parameter.HeadPump = 0.161*HeadPump_Avr;
		//printf("\n\r The HeadPump_Avr is %f.2\r\n", Coeff_CTP*HeadPump_Avr);
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
