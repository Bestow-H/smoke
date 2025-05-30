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
  uint16_t rawEncode;     // ���������ص�ԭʼֵ
  uint16_t lastRawEncode; // ��һ�εı�����ԭʼֵ
  int32_t round;         // Ȧ��
  float conEncode;     // �����������ı�����ֵ
  float lastConEncode; // ��һ�εĴ���������������ֵ

  int16_t rawSpeed;     // ���ص�ת��
  int16_t lastRawSpeed; // ��һ�η��ص�ת��

  int16_t Current; // ת�ص���

  int8_t temp; // �¶�
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