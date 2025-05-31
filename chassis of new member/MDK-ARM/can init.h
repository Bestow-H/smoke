#ifndef _CANINIT_H_
#define _CANINIT_H_
#include "usart.h"
#include "can.h"
struct Motor
{
  int16_t rawEncode;     // 
  int16_t lastRawEncode; // 
  int32_t round;         // 
  int32_t conEncode;     // 
  int32_t lastConEncode; // 

  int16_t rawSpeed;     // 
  int16_t lastRawSpeed; // 

  int16_t Current; // 

  int8_t temp; //
};

struct Motor4005
{
  uint16_t rawEncode;     // 编码器返回的原始值
  uint16_t lastRawEncode; // 上一次的编码器原始值
  int32_t round;         // 圈数
  float conEncode;     // 处理后的连续的编码器值
  float lastConEncode; // 上一次的处理后的连续编码器值

  int16_t rawSpeed;     // 返回的转速
  int16_t lastRawSpeed; // 上一次返回的转速

  int16_t Current; // 转矩电流

  int8_t temp; // 温度
  int ID;
  uint8_t State;
		
};
//void RGB_SetColor(uint16_t color);
typedef struct
{
  struct Motor data4005;
}Motor_typedef4005;



typedef struct
{
  // uint8_t gyroStatus[2];;

  struct Motor data;
 
}Motor_typedef;

void CAN_send(CAN_HandleTypeDef *_hcan, int16_t stdid, int16_t num1, int16_t num2, int16_t num3, int16_t num4);
void CAN_Filter_Init(CAN_HandleTypeDef *hcan);
void LKMF_Data_Read(CAN_HandleTypeDef* hcan, uint16_t motor_id);
void LKMF_iq_ctrl(CAN_HandleTypeDef* hcan, uint16_t motor_id, int32_t iqControl);

#endif