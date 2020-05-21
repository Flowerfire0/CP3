/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
uint16_t Coeff_Land_Can = 100;
uint16_t Coeff_head_Locked_Can = 70;
uint16_t LHQ_ratio_Can;
uint16_t CT_length_ratio_Can = 100;
uint16_t XZ_length_ratio_Can = 100;
uint16_t CTZX_Can = 100;
uint16_t XZZX_Can = 100;
uint16_t temp_f = 100;
uint16_t BackTransmissionRatio_Can = 1524,FrontTransmissionRatio_Can = 1326;
uint32_t HarvestedLand=0,TotalHarvestedLand=0;
uint32_t HarvestedLand_old=0, TotalHarvestedLand_old=0;

uint16_t MachineID;
uint8_t MachineID_H,MachineID_L;

float Coeff_Land =1.0;

float Coeff_head_Locked =0.7;
float Coeff_Travel_head_ratio=2.0;

float LHQ_ratio=0.9;
float CT_length_ratio=1.0,XZ_length_ratio=1.0;
float CTZX = 1.0,XZZX = 1.0;
uint16_t InGearN = 44,FGearN = 72,BGearN = 65;

uint16_t CTS_SET1 = 1000,CTS_SET2 = 1200,CTS_SET3 = 1300,CTS_SET4 = 1400,CTS_SET5 = 1450,CTS_SET6 = 1500;

CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;


uint8_t               TxData[8];
uint32_t              TxMailbox;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 9;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_8TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration    
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */
  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */


void CAN_Transmit1(void)
{
	TxHeader.StdId = 0;
	TxHeader.ExtId = 0x0301F5E0;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;		
	
	TxData[0] = AlarmStatusDate0;
	TxData[1] = AlarmStatusDate1;
	TxData[2] = AlarmStatusDate2;
	TxData[3] = AlarmStatusDate3;	
	TxData[4] = AlarmStatusDate4;
	TxData[5] = 0xFF;
	TxData[6] = 0xFF;
	TxData[7] = 0xFF;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		Error_Handler();
	}		
}

void CAN_Transmit2(void)
{
	TxHeader.StdId = 0;
	TxHeader.ExtId = 0x0302F5E0;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;		
	
	TxData[0] = (uint8_t)((XZS_Disp>>8)&0x0FF);
	TxData[1] = (uint8_t)(XZS_Disp&0x0FF); 
	TxData[2] = (uint8_t)((FJS_Disp>>8)&0x0FF);
	TxData[3] = (uint8_t)(FJS_Disp&0x0FF); 	
	TxData[4] = (uint8_t)((cp3_parameter.Oil_Temp>>8)&0x0FF);//油温
	TxData[5] = (uint8_t)(cp3_parameter.Oil_Temp&0x0FF);
	TxData[6] = 0xFF;
	TxData[7] = 0xFF;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		Error_Handler();
	}		
}

void CAN_Transmit3(void)
{
	TxHeader.StdId = 0;
	TxHeader.ExtId = 0x0303F5E0;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;		
	
	TxData[0] = (uint8_t)((cp3_parameter.Water_Pres>>8)&0x0FF);
	TxData[1] = (uint8_t)(cp3_parameter.Water_Pres&0x0FF);
	TxData[2] = 0XFF;
	TxData[3] = VerYear;	
	TxData[4] = VerMonth;
	TxData[5] = VerDay;
	TxData[6] = version1;
	TxData[7] = version2;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		Error_Handler();
	}		
}

void CAN_Transmit4(void)
{
	TxHeader.StdId = 0;
	TxHeader.ExtId = 0x0304F5E0;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;		
	
	TxData[0] = (uint8_t)((HarvestedLand>>24)&0x0FF);
	TxData[1] = (uint8_t)((HarvestedLand>>16)&0x0FF);
	TxData[2] = (uint8_t)((HarvestedLand>>8)&0x0FF); 
	TxData[3] = (uint8_t)(HarvestedLand&0x0FF);	
	TxData[4] = (uint8_t)((TotalHarvestedLand>>24)&0x0FF);
	TxData[5] = (uint8_t)((TotalHarvestedLand>>16)&0x0FF);
	TxData[6] = (uint8_t)((TotalHarvestedLand>>8)&0x0FF);
	TxData[7] = (uint8_t)(TotalHarvestedLand&0x0FF);
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		Error_Handler();
	}		
}

void CAN_Transmit5(void)
{
	TxHeader.StdId = 0;
	TxHeader.ExtId = 0x0305F5E0;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;		
	
	TxData[0] = (uint8_t)((CTFS_Disp1>>8)&0x0FF);
	TxData[1] = (uint8_t)(CTFS_Disp1&0x0FF);
	TxData[2] = (uint8_t)((CTBS_Disp1>>8)&0x0FF);
	TxData[3] = (uint8_t)(CTBS_Disp1&0x0FF);	
	TxData[4] = (uint8_t)((CTFS_Disp2>>8)&0x0FF);;
	TxData[5] = (uint8_t)(CTFS_Disp2&0x0FF);
	TxData[6] = (uint8_t)((CTBS_Disp2>>8)&0x0FF);
	TxData[7] = (uint8_t)(CTBS_Disp2&0x0FF);
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		Error_Handler();
	}		
}

void CAN_Transmit6(void)
{
	TxHeader.StdId = 0;
	TxHeader.ExtId = 0x0306F5E0;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;		
	
	TxData[0] = (uint8_t)((CTFS_Disp3>>8)&0x0FF);
	TxData[1] = (uint8_t)(CTFS_Disp3&0x0FF);
	TxData[2] = (uint8_t)((CTBS_Disp3>>8)&0x0FF);
	TxData[3] = (uint8_t)(CTBS_Disp3&0x0FF);	
	TxData[4] = (uint8_t)((CTS_Disp>>8)&0x0FF);
	TxData[5] = (uint8_t)(CTS_Disp&0x0FF);
	TxData[6] = 0XFF;
	TxData[7] = 0XFF;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request Error */
		Error_Handler();
	}		
}

//void CAN_Transmit7(void)
//{
//	TxHeader.StdId = 0;
//	TxHeader.ExtId = 0x0306F5E0;
//  TxHeader.RTR = CAN_RTR_DATA;
//  TxHeader.IDE = CAN_ID_EXT;
//  TxHeader.DLC = 8;		

//	static uint8_t abc = 0;
//	abc++;
//	AT24CXX_WriteLenByte(0xA0, abc, 4);
//	uint8_t bcd = AT24CXX_ReadLenByte(0xA0, 4);
//	
//	TxData[0] = bcd;
//	TxData[1] = 0XFF;
//	TxData[2] = 0XFF;
//	TxData[3] = 0XFF;	
//	TxData[4] = 0XFF;
//	TxData[5] = 0XFF;
//	TxData[6] = 0XFF;
//	TxData[7] = 0XFF;
//	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK)
//	{
//		/* Transmission request Error */
//		Error_Handler();
//	}		
//}



//CAN中断处理过程
//此函数会被CAN_Receive_IT()调用
//hcan:CAN句柄
uint8_t  RxData[8];
uint32_t IDmask;
extern uint8_t eeprom_write_flag2;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	
	
//    __HAL_CAN_ENABLE_IT(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);//重新开启FIF00消息挂号中断

  /* Get RX message */
	
//	__disable_irq();
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
		return;
  }
	
	if(RxHeader.IDE == CAN_ID_EXT)                    //确保接收到扩展帧数据
	{
		//IDmask=RxHeader.ExtId & 0x00FFFF00;
		//switch(RxHeader.ExtId & 0x00FFFF00)
		switch(RxHeader.ExtId)	
		{
			case 0x0300E0F5:
					if(RxData[0] & 0x01) 
					{
						HarvestedLand=0;
//						AT24CXX_WriteLenByte(0x80,HarvestedLand,4);
					}
			break;			
			
			case 0x0301E0F5:
				BackTransmissionRatio_Can = (RxData[0]<<8)+RxData[1];
				FrontTransmissionRatio_Can = (RxData[2]<<8)+RxData[3];				
				LHQ_ratio_Can=(RxData[4]<<8)+RxData[5];//离合器报警系数
				//Coeff_Land_Can=(RxData[6]<<8)+RxData[7];
				OilTempAlarm=(RxData[6]<<8)+RxData[7];
			eeprom_write_flag2=1;
//				AT24CXX_WriteLenByte(0x00,BackTransmissionRatio_Can,2);
//				AT24CXX_WriteLenByte(0x04,FrontTransmissionRatio_Can,2);
//				AT24CXX_WriteLenByte(0x0C,LHQ_ratio_Can,2);
//				AT24CXX_WriteLenByte(0x44,OilTempAlarm,2);
			break;			
			
//			case 0x02E0:
//			//case	0x0302E0F5:
//				OilTempAlarm = (RxData[0]<<8)+RxData[1];	//油温报警值			
//				MachineID=(RxData[2]<<8)+RxData[3];	 //车辆ID				
//				AT24CXX_WriteLenByte(0x44,OilTempAlarm,2);
//				AT24CXX_WriteLenByte(0x18,MachineID,2);	
//							
//			break;

			case 0x0303E0F5:
				//Coeff_head_Locked_Can=(RxData[2]<<8)+RxData[3];	
				//Coeff_Travel_head_ratio_Can=(RxData[0]<<8)+RxData[1];	
				CTS_SET5=(RxData[4]<<8)+RxData[5];
				CTS_SET6=(RxData[6]<<8)+RxData[7];			
				
//				AT24CXX_WriteLenByte(0x3C,CTS_SET5,2);
//				AT24CXX_WriteLenByte(0x40,CTS_SET6,2);			
			break;		

			case 0x0304E0F5:
				CTS_SET1=(RxData[0]<<8)+RxData[1];
				CTS_SET2=(RxData[2]<<8)+RxData[3];
				CTS_SET3=(RxData[4]<<8)+RxData[5];
				CTS_SET4=(RxData[6]<<8)+RxData[7];
//				AT24CXX_WriteLenByte(0x2C,CTS_SET1,2);
//				AT24CXX_WriteLenByte(0x30,CTS_SET2,2);
//				AT24CXX_WriteLenByte(0x34,CTS_SET3,2);
//				AT24CXX_WriteLenByte(0x38,CTS_SET4,2);				
			break;			
			
			case 0x0305E0F5:
				//CT_length_ratio_Can=(RxData[2]<<8)+RxData[3];
				XZZX_Can=(RxData[4]<<8)+RxData[5];			
				temp_f = (RxData[6]<<8)+RxData[7];
				if(temp_f>30 && temp_f<=100)
					XZ_length_ratio_Can = temp_f;
				else
					XZ_length_ratio_Can = 100;
				
				
//				AT24CXX_WriteLenByte(0x14,XZZX_Can,2);
//				AT24CXX_WriteLenByte(0x10,XZ_length_ratio_Can,2);
			break;							
			
			case 0x0306E0F5:
				Coeff_head_Locked_Can=(RxData[0]<<8)+RxData[1];
				InGearN=((RxData[2]<<8)+RxData[3]);	
				FGearN=((RxData[4]<<8)+RxData[5]);	
				BGearN=((RxData[6]<<8)+RxData[7]);
				
			
//				AT24CXX_WriteLenByte(0x1C,Coeff_head_Locked_Can,2);
//				AT24CXX_WriteLenByte(0x20,InGearN,2);
//				AT24CXX_WriteLenByte(0x24,FGearN,2);
//				AT24CXX_WriteLenByte(0x28,BGearN,2);
			break;		

			case 0x0309E0F5:
				if(RxData[0] & 0x01) 
				{
					HarvestedLand=0;
					TotalHarvestedLand=0;
					TotalHarvestedLand_old = 0;
					
//					AT24CXX_WriteLenByte(0x80,HarvestedLand,4);	
//					AT24CXX_WriteLenByte(0x84,TotalHarvestedLand,4);	
				}
			break;					
				
			default:
			break;
			
		}
	}
	
	for(int i=0;i<8;i++)
	{
		RxData[i] = 0;
	}
	RxHeader.ExtId = 0;
	
//	__enable_irq();
}



void CAN_User_Init(CAN_HandleTypeDef* hcan )   //用户初始化函数
{
 CAN_FilterTypeDef  sFilterConfig;
 HAL_StatusTypeDef  HAL_Status;
	
//  TxMeg.IDE=CAN_ID_STD;//CAN_ID_EXT;
//  TxMeg.RTR=CAN_RTR_DATA;
	
  sFilterConfig.FilterBank = 0;                       //过滤器0
  sFilterConfig.FilterMode =  CAN_FILTERMODE_IDMASK;  //设为列表模式    
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    
		
  sFilterConfig.FilterIdHigh = 0;   
  sFilterConfig.FilterIdLow  = 0;    
	
  sFilterConfig.FilterMaskIdHigh =0;
  sFilterConfig.FilterMaskIdLow  =0; 
	
//	sFilterConfig.FilterIdHigh =((0x030000F5<<3) >>16) &0xffff;//数组任意一个成员都可以作为验证码
//  sFilterConfig.FilterIdLow =((0x030000F5<<3)&0xffff) | CAN_ID_EXT;

	
	
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;    //接收到的报文放入到FIFO0中 
 
  sFilterConfig.FilterActivation = ENABLE;  	//不激活过滤器
  sFilterConfig.SlaveStartFilterBank  = 14; 
 
  HAL_Status=HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
  HAL_Status=HAL_CAN_Start(&hcan1);  //开启CAN
  if(HAL_Status!=HAL_OK){
		printf("开启CAN失败\r\n");	
	}	
	HAL_Status=HAL_CAN_ActivateNotification(&hcan1,   CAN_IT_RX_FIFO0_MSG_PENDING);
	if(HAL_Status!=HAL_OK){
		printf("开启挂起中段允许失败\r\n");	
  }
}



/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
