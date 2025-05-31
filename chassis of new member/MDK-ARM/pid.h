#ifndef __PID_H
#define __PID_H
 
#include "main.h"
#include <math.h>

 
// ??PID??????
typedef struct {
    float Kp;  // ????
    float Ki;  // ????
    float Kd;  // ????
    float setpoint;  // ???
    float error_sum;  // ?????
    float last_error;  // ??????
	  float error;
} PIDController;
extern PIDController PID;
extern PIDController PID1;
extern PIDController PID2;
extern PIDController PID3;
extern PIDController PID4;
float PIDController_computeouter(PIDController* pid, float input, float dt,float Kp, float Ki, float Kd, float setpoint);
float PIDController_computeinner(PIDController* pid1, float input, float dt,float Kp, float Ki, float Kd, float setpoint);
#endif
