#ifndef _HUIDIAO_H_
#define _HUIDIAO_H_
#include "usart.h"
#include "can.h"
#include "yaokong.h"
#include "can init.h"
extern uint8_t R[4];
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif