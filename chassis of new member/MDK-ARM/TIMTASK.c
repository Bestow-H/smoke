#include "yaokong.h"
#include "tim.h"
#include "can init.h"
#include "timtask.h"
#include "pid.h"
#include "motor.h"
#include "can.h"
#include "vofa.h"
#include "chassis.h"
int16_t V1,V2,V3,V4;
float target;
uint8_t inspectmode=0;
uint16_t inspectime=0;
uint16_t keytime=0;
uint16_t xunjiantime;
extern uint8_t uart_status;
extern uint16_t shijue;
int16_t AA          = 0;
uint8_t DBUS_status = 0;
uint8_t DBUS_Time   = 100;
float  z=0;
PIDController PID;
PIDController PID1;
PIDController PID2;
PIDController PID3;
PIDController PID4;
PIDController PID5;
PIDController PID6;
PIDController PID7;
extern Motor_typedef GM6020_rx;
extern Motor_typedef GM6020_angel;
extern Motor_typedef GM3508_rx1;
extern Motor_typedef GM3508_rx2;
extern Motor_typedef GM3508_rx3;
extern Motor_typedef GM3508_rx4;
extern remote_typedef remote_t; 
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) { 
        
			  uart_status++;
			if(uart_status>=200)
			{uart_status=200;}
			   if( HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{
				  keytime++;
				if(keytime>3&&HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{inspectmode=!inspectmode;
					while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
				{}}

				}
			else
     {keytime=0;}
	if(inspectmode==0)
{		if(shijue==640)
	  {HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);}
		else
		{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);}
			
	 if(uart_status<10)
	 {
		z=((float)shijue-640)*0.8;
		 
  CAN_send(&hcan, 0x200, 0,
PIDController_computeouter(&PID, GM3508_rx1.data.rawEncode, 0.05,2.5,0, 0,GM3508_rx1.data.rawEncode+z),
		0	,0);
		// CAN_send(&hcan, 0x200, 0,1500,0,0);
	 }
	 else
	 {
   //z=0;
		 CAN_send(&hcan, 0x200, 0,0,0,0);
	}
}	
			
		//Ñ²¼ì
	if(inspectmode==1)
{
	if(shijue==640&&inspectime<=14)
 	{ xunjiantime++;
if(xunjiantime<=500)
{ CAN_send(&hcan, 0x200, 0,
  PIDController_computeouter(&PID, GM3508_rx1.data.rawEncode, 0.05,2,0, 0,GM3508_rx1.data.rawEncode-260),
		0	,0);HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);}
if(xunjiantime>500&&xunjiantime<1000)
{CAN_send(&hcan, 0x200, 0,
  PIDController_computeouter(&PID, GM3508_rx1.data.rawEncode, 0.05,2,0, 0,GM3508_rx1.data.rawEncode+260),
		0	,0);HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);}
if(xunjiantime>1000)
xunjiantime=0;}
	if(shijue!=640)
	{
		inspectime=100;
  	xunjiantime=0;
		
	}
	if(inspectime>9)
	{  
		inspectime--;
		if(inspectime<=15)
		{ inspectime=8;}
		if(uart_status<10)
		 {
			z=((float)shijue-640)*0.8;
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
  CAN_send(&hcan, 0x200, 0,
  PIDController_computeouter(&PID, GM3508_rx1.data.rawEncode, 0.05,2.5,0, 0,GM3508_rx1.data.rawEncode+z),
		0	,0);
		// CAN_send(&hcan, 0x200, 0,1500,0,0);
    	 }
//	 else
//	 {
//   //z=0;
//		 CAN_send(&hcan, 0x200, 0,0,0,0);
//	}
}}
			
			
			
			
			
			
			//CAN_send(&hcan, 0x200, 0,00,PIDController_computeinner(&PID2, GM3508_rx3.data.rawSpeed, 0.05,7,0, 0,PIDController_computeouter(&PID3, GM3508_rx3.data.rawEncode, 0.05,6,0, 0,GM3508_rx3.data.rawEncode+1500)),00);//LKMF_Data_Read(&hcan,2);
					// LKMF_iq_ctrl(&hcan,2,200);
//     chassis(remote_t.control.ch3_int16,remote_t.control.ch2_int16,remote_t.control.ch0_int16,3,3,3);
		//CAN_send(&hcan, 0x1FF, V1,5000,V3,V4);
//        CAN_send(&hcan, 0x200,
//					PIDController_computeinner(&PID1, GM3508_rx1.data.rawSpeed, 0.05,5, 0.1, 0.05, 0),
//					PIDController_computeinner(&PID2, GM3508_rx2.data.rawSpeed, 0.05,4, 0.1, 0.05, 0),
//					PIDController_computeinner(&PID3, GM3508_rx3.data.rawSpeed, 0.05,4, 0.1, 0.05, 3000),
//				3000,	PIDController_computeinner(&PID4, GM3508_rx4.data.rawSpeed, 0.05,5, 0.1, 0.05, 0));
//        //    CAN_send(&hcan,0x1FF,AA,0,0,0);PIDController_computeinner(&PID1, GM6020_rx.data.rawSpeed, 0.05,70,0.5, 0,PIDController_computeouter(&PID, GM6020_rx.data.rawEncode, 0.05,0.05,0, 0,000))
//        VOFA_justfloat(0,
//                       remote_t.control.ch2_int16,
//                       remote_t.control.ch3_int16,
//                       PID.setpoint,
//                       GM6020_rx.data.rawEncode,
//                       0,
//                       0,
//                       0,
//                       0,
//                       0);
   //CAN_send(&hcan, 0x200,0,0,500,0);
  // CAN_send(&hcan, 0x200,0,0,PIDController_computeinner(&PID, GM3508_rx3.data.rawSpeed, 0.05,15,0, 0,PIDController_computeouter(&PID1, GM3508_rx3.data.rawEncode, 0.05,8,0, 0,GM3508_rx3.data.rawEncode-350)),0);
//			if(remote_t.control.s1_u8==3)
//			{ 
//				CAN_send(&hcan,0x200,
//				PIDController_computeinner(&PID, GM3508_rx1.data.rawSpeed, 0.05,60,0,0,-6800),
//				0,
//				PIDController_computeinner(&PID2, GM3508_rx3.data.rawSpeed, 0.05,10,0, 0,PIDController_computeouter(&PID3, GM3508_rx3.data.rawEncode, 0.05,8,0, 0,GM3508_rx3.data.rawEncode-150)),
//				PIDController_computeinner(&PID4, GM3508_rx4.data.rawSpeed, 0.05,70,0, 0,6800));  
//			z++;
//				if((-200<GM3508_rx3.data.rawSpeed)&&(GM3508_rx3.data.rawSpeed<50))
//				{
//				  z++;
//					if(z>=1000)
//					{
//					  CAN_send(&hcan,0x200,0,0,0,0);
//						z--;
//					 }
//				 }
//				z=0;
//			}
			
//			if(remote_t.control.s1_u8!=3)
//			{
//			CAN_send(&hcan,0x200,0,0,0,0);
//			}
			}//PIDController_computeinner(&PID1, GM6020_rx.data.rawSpeed, 0.05,1,0, 0,PIDController_computeouter(&PID, GM6020_rx.data.rawEncode, 0.05,30,0.005, 0,2000))
    if (htim->Instance == TIM4) { // ??????
        DBUS_Time++;
        if (DBUS_Time >= 50)
            DBUS_status = off_line;
        else
            DBUS_status = on_Line;
        if (DBUS_status == off_line) {
            DBUS_offLine();
        }
    }
}
