#include "main.h"
#include <math.h>
#include "pid.h" 

 void gothrough0(PIDController* a)//¹ýÁã½âËã
 {
	if(a->error<-4092)
		{a->error+=8191;}
  if(a->error>4092)
    {a->error-=8191;}
 }
 
float PIDController_computeouter(PIDController* pid, float input, float dt,float Kp, float Ki, float Kd, float setpoint)
 {   
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->setpoint = setpoint;
    pid->error_sum = 0;
    pid->last_error = 0;
	 
    pid->error= pid->setpoint - input; 
	  gothrough0(pid);
    pid->error_sum += pid->error * dt;
    float error_delta = (pid->error - pid->last_error) / dt;
    float output = pid->Kp * pid->error+ pid->Ki * pid->error_sum + pid->Kd * error_delta;
    pid->last_error = pid->error;
	  if(output<-29000)
			 output=-29000;
		if(output>29000)
			 output=29000;
    return output;
}
 float PIDController_computeinner(PIDController* pid1, float input, float dt,float Kp, float Ki, float Kd, float setpoint)
 {   
    pid1->Kp = Kp;
    pid1->Ki = Ki;
    pid1->Kd = Kd;
    pid1->setpoint = setpoint;
    pid1->error_sum = 0;
    pid1->last_error = 0;
	 
    pid1->error = pid1->setpoint - input;
	
    pid1->error_sum += pid1->error * dt;
    float error_delta = (pid1->error - pid1->last_error) / dt;
    float output1 = pid1->Kp * pid1->error+ pid1->Ki * pid1->error_sum + pid1->Kd * error_delta;
    pid1->last_error = pid1->error;
    if(output1<-15000)
			 output1=-15000;
		if(output1>15000)
			 output1=15000;
    return output1;
}
 
 

 
 
 
 
 