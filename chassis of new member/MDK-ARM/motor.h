#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "main.h"
#include "can init.h"

void MotorResolve(Motor_typedef *motor, uint8_t *RxMessage);
void MotorRoundResolve(Motor_typedef *motor);
void LK_MotorResolve(Motor_typedef4005 *motor,uint8_t *RxMessage);
#endif