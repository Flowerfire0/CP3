#include "user_config.h"

uint16_t CTS_set = 0;
uint8_t Flag_Median = 0;
uint8_t flag_Reverse = 0;

void TravelControl(void)
{
	if(cp3_parameter.ControllerHandle < 245)
	{
		flag_Reverse = 1;
		DC_Con(1);		
	}
	else
	{
		flag_Reverse = 0;
		DC_Con(0);
	}	
	if(cp3_parameter.ControllerHandle >=246 && cp3_parameter.ControllerHandle <= 255)//中位
	{
		Flag_Median = 1;
		ZW_Con(1);
		if(cp3_parameter.TravelPump<245)
		{
			XZF_Con(1);
			XZB_Con(0);			
		}
		else if(cp3_parameter.TravelPump>255)
		{
			XZF_Con(0);
			XZB_Con(1);			
		}			
		else
		{
			XZF_Con(0);
			XZB_Con(0);				
		}		
	}
	else
	{
		Flag_Median = 0;
		ZW_Con(0);
		if(cp3_parameter.TravelPump < (cp3_parameter.ControllerHandle-5))
		{
			XZF_Con(1);
			XZB_Con(0);		
		}
		else if(cp3_parameter.ControllerHandle > (cp3_parameter.ControllerHandle+5))
		{
			XZF_Con(0);
			XZB_Con(1);			
		}			
		else
		{
			XZF_Con(0);
			XZB_Con(0);				
		}			
	}
}

void HeadControl(void)
{
	uint16_t HeadLock = 0;
	Coeff_head_Locked = 0.01*Coeff_head_Locked_Can;
	HeadLock = (450-250)*Coeff_head_Locked+250;
	if(cp3_parameter.Locked == 0)//锁定		
	{
		if(cp3_parameter.HeadPump < (HeadLock-5)) 
		{
			CTF_Con(1);
			CTB_Con(0);
		}
		else if(cp3_parameter.HeadPump > (HeadLock+5))
		{
			CTF_Con(0);
			CTB_Con(1);
		}
		else
		{
			CTF_Con(0);
			CTB_Con(0);
		}			
	}
	else
	{
		if(CTS_Disp < (CTS_set-10) && cp3_parameter.HeadPump < 450)//实际速度大于设定速度且推杆没有到最大，伸出
		{
			CTF_Con(1);
			CTB_Con(0);			
		}
		else if(CTS_Disp > (CTS_set+10) && cp3_parameter.HeadPump > 250)//实际速度小于设定速度且推杆没有到中位，缩回
		{
			CTF_Con(0);
			CTB_Con(1);		
		}			
		else
		{
			CTF_Con(0);
			CTB_Con(0);					
		}						
	}
}

//void TravelControl(void)
//{
//	float handle=0,travel=0;
//	float handle1=0,handle2=0;	
//	
//	XZ_length_ratio = 0.01*XZ_length_ratio_Can;
//	XZZX = 0.01*XZZX_Can;

//	handle = (float)(cp3_parameter.ControllerHandle - 50.0)/(435-50);//50  435 
//	if(handle<0)
//	{
//		handle=0;
//	}
//	else if(handle>1)
//	{
//		handle=1;
//	}

//	if(handle <0.5 - Vol_red2)//输出倒车信号
//	{
//		flag_Reverse = 1;
//		DC_Con(1);
//	}
//	else
//	{
//		flag_Reverse = 0;
//		DC_Con(0);
//	}
//	
//	if((cp3_parameter.TravelPump <LOW))
//	{		
//		cp3_parameter.TravelPump = LOW;
//		XZF_Con(0);
//		XZB_Con(0);
//	}
//	else if(cp3_parameter.TravelPump >HIGH)
//	{  
//		cp3_parameter.TravelPump = HIGH;	
//		XZF_Con(0);
//		XZB_Con(0);		
//	}
//	travel =(float) (cp3_parameter.TravelPump - LOW) /(HIGH-LOW);
//	
////	printf("\n\r The a is %d", a);	
////	printf("\n\r The b is %d", b);
//////	
////	printf("\n\r The HeadPump is %ld", 	cp3_parameter.HeadPump);
//////	printf("\n\r The TravelPump is %ld", 	cp3_parameter.TravelPump);
//////	printf("\n\r The ControllerHandle is %ld",  cp3_parameter.ControllerHandle);
////	printf("\n\r The head is %f", head);
////	
////	printf("\n\r The 	handle is %f",	handle);
//	
//	if((handle >=(0.5 - Vol_red2)) && (handle <= (0.5 + Vol_red2)))//操作手柄中位 2.4V~2.6V
//	{
//		Flag_Median = 1;
//		ZW_Con(1);
////	  printf("travel%f\r\n",travel);
//		if((0.5*XZZX-travel)>Vol_red3)
//		{
//					//printf("111%f\r\n",(0.5*XZZX-travel));
//			//printf("111\r\n");
//			XZF_Con(1);
//			XZB_Con(0);
//			//delay_ms((MED-TravelPump)*50);				
//		}
//		else if((travel-0.5*XZZX)>Vol_red3)
//		{
//					//printf("222%f\r\n",(0.5*XZZX-travel));
//			//printf("222\r\n");
//			XZF_Con(0);
//			XZB_Con(1);
//			//delay_ms((TravelPump-MED)*50);				
//		}			
//		else
//		{
//					//printf("333%f\r\n",(0.5*XZZX-travel));
//			//printf("333\r\n");
//			XZF_Con(0);
//			XZB_Con(0);				
//		}			
//	}
//	else 
//	{
//		Flag_Median = 0;
//		handle1=(handle-0.5)*XZ_length_ratio+0.5;
//		
//		ZW_Con(0);
//		if((handle1-travel)>Vol_red)
//		{
//			XZF_Con(1);
//			XZB_Con(0);
//			//delay_ms((MED-TravelPump)*50);				
//		}
//		else if((travel-handle1)>Vol_red)
//		{
//			XZF_Con(0);
//			XZB_Con(1);
//			//delay_ms((TravelPump-MED)*50);				
//		}			
//		else
//		{
//			XZF_Con(0);
//			XZB_Con(0);				
//		}	
//		
//	}
//}


//void HeadControl(void)
//{
//	float head=0.0, headcontrol = 0.0;
//	Coeff_head_Locked = 0.01*Coeff_head_Locked_Can;
////	CT_length_ratio = 0.01*CT_length_ratio_Can;	
//	CTZX = 0.01*CTZX_Can;
//	if((cp3_parameter.HeadPump <LOW))
//	{		
//		cp3_parameter.HeadPump = LOW;
//		CTF_Con(0);
//		CTB_Con(0);
//	}
//	else if((cp3_parameter.HeadPump >HIGH))
//	{		
//		cp3_parameter.HeadPump = HIGH;
//		CTF_Con(0);
//		CTB_Con(0);
//	}
//	//默认最低为30，最高位210，即CT_length_ratio=1；	
//	
//	head =(float)(cp3_parameter.HeadPump - LOW )/(HIGH- LOW);	
//	//printf("head:%f\r\n",head);
//	if(head>1)
//	{
//		head=1;
//	}
//	
////if (cp3_parameter.Locked ==0 )//锁定
////	{
////		if((Coeff_head_Locked-head)>Vol_red)      //#define  LOCK		0.64  锁定可调 
////		{
////			CTF_Con(1);
////			CTB_Con(0);
////		}
////		else if((head-Coeff_head_Locked)>Vol_red)
////		{
////			CTF_Con(0);
////			CTB_Con(1);
////		}
////		else
////		{
////			CTF_Con(0);
////			CTB_Con(0);
////		}
////	}
////	else
////	{
////		if(handle <=(0.5 + Vol_red2))//操作手柄中位 及一下，采头不动
////		{
////			if((0.5*CTZX-head)>Vol_red3)
////			{
////				CTF_Con(1);
////				CTB_Con(0);	
////				//printf("CTB1\r\n");
////			}
////			else if((head-0.5*CTZX)>Vol_red3)
////			{
////				CTF_Con(0);
////				CTB_Con(1);		
////				//printf("CTB2\r\n");
////			
////			}			
////			else
////			{
////				CTF_Con(0);
////				CTB_Con(0);	
////				//printf("CTB3\r\n");				
////			}
////			//printf("head-0.5*CTZX:%f\r\n",head);				
////		}
////		else //非中位
////		{
////					
////			handle2=(handle-0.5)*Coeff_Travel_head_ratio+0.5;
//////			if(handle2>1)
//////			{
//////				handle2=1;
//////			}
////			if((handle2>=CT_length_ratio)&&(head>CT_length_ratio-Vol_red))
////			{
////				CTF_Con(0);
////				CTB_Con(0);	
////				//printf("CTB4\r\n");	
////			}
////			else
////			{
////				if((handle2-head)>Vol_red)
////				{
////					CTF_Con(1);
////					CTB_Con(0);		
////				//printf("CTB5\r\n");	
////				}
////				else if((head-handle2)>Vol_red)
////				{
////					CTF_Con(0);
////					CTB_Con(1);	
////					//printf("CTB6\r\n");	
////				}			
////				else
////				{
////					CTF_Con(0);
////					CTB_Con(0);
////					//printf("CTB7\r\n");						
////				}			
////			}
////		}
////	}

//	if (cp3_parameter.Locked == 0)//锁定		
//	{
//		if((Coeff_head_Locked-head)>Vol_red)      //#define  LOCK		0.64  锁定可调 
//		{
//			CTF_Con(1);
//			CTB_Con(0);
//		}
//		else if((head-Coeff_head_Locked)>Vol_red)
//		{
//			CTF_Con(0);
//			CTB_Con(1);
//		}
//		else
//		{
//			CTF_Con(0);
//			CTB_Con(0);
//		}			
//	}
//	else
//	{
//		headcontrol = (CTS_set+CTS_max)/(2*CTS_max);
//		if(headcontrol < 0.5 || flag_Reverse == 1)
//			headcontrol = 0.5;
//		if((headcontrol-head)>Vol_red)
//		{
//			CTF_Con(1);
//			CTB_Con(0);			
//		}
//		else if((head-headcontrol)>Vol_red)
//		{
//			CTF_Con(0);
//			CTB_Con(1);		
//		}			
//		else
//		{
//			CTF_Con(0);
//			CTB_Con(0);					
//		}						
//	}
//}

