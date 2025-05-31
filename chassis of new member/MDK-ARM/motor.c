#include "Motor.h"
#include "main.h"

void MotorRoundResolve(Motor_typedef *motor)
{
    if (motor->data.rawEncode - motor->data.lastRawEncode < -4096) {
        motor->data.round--;
    }
    if (motor->data.rawEncode - motor->data.lastRawEncode > 4096) {
        motor->data.round++;
    }
    motor->data.conEncode=motor->data.round*8192+motor->data.rawEncode;
}

void MotorResolve(Motor_typedef *motor, uint8_t *RxMessage)
{
    /*??????*/
    motor->data.lastRawEncode = motor->data.rawEncode;
    motor->data.rawEncode     = ((uint16_t)RxMessage[0] << 8 | (uint16_t)RxMessage[1]);
    /*????*/
    motor->data.lastRawSpeed = motor->data.rawSpeed;
    motor->data.rawSpeed     = ((uint16_t)RxMessage[2] << 8 | (uint16_t)RxMessage[3]);
    /*????*/
    motor->data.Current = ((uint16_t)RxMessage[4] << 8 | (uint16_t)RxMessage[5]);
    /*????*/
    motor->data.temp = RxMessage[6];//ÎÂ¶È
	MotorRoundResolve(motor);
}
void LK_MotorResolve(Motor_typedef4005 *motor,uint8_t *RxMessage)
{                                
	if(RxMessage[0]==0x9c)  
	{
		motor->data4005.temp = RxMessage[1];
	
		motor->data4005.Current = ((uint16_t)RxMessage[3] << 8 | (uint16_t)RxMessage[2]);
	
		motor->data4005.lastRawSpeed = motor->data4005.rawSpeed;
		motor->data4005.rawSpeed     = ((uint16_t)RxMessage[5] << 8 | (uint16_t)RxMessage[4]);
		
		motor->data4005.lastRawEncode = motor->data4005.rawEncode;
		motor->data4005.rawEncode     = ((uint16_t)RxMessage[7] << 8 | (uint16_t)RxMessage[6]);
		
//		if(motor->data4005.State)
//		{
//			if (motor->data4005.rawEncode - motor->data4005.lastRawEncode < -40000) 
//			{
//				motor->data4005.round++;
//			}
//			else if (motor->data4005.rawEncode - motor->data4005.lastRawEncode > 40000) 
//			{
//				motor->data4005.round--;
//			}
//			motor->data4005.lastConEncode = motor->data4005.conEncode;
//			motor->data4005.conEncode     = (float) motor->data4005.round* 360 + (float) motor->data4005.rawEncode * 360 / 65536;	
//		}
//		else
//		{
//			motor->data4005.conEncode=(float)motor->data4005.rawEncode * 360 / 65536;	
//		} 
	}
	

}
