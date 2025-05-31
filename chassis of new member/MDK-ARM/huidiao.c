#include <main.h>
#include "usart.h"
#include <yaokong.h>
#include "yaokong.h"
#include "can init.h"
extern uint8_t R[4];
uint16_t shijue=640;
uint8_t uart_status=0;
extern uint8_t DBUS_Time;
extern float z;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		if(huart->Instance == USART1)
		{
	 if( R[0] == 0xcd && R[3] == 0xdc)
			{
			  shijue=((int16_t)R[2]<<8 | (int16_t)R[1]);
				
				uart_status=0;
			}
		 HAL_UART_Receive_DMA(&huart1,R,sizeof(R));
	}
	}