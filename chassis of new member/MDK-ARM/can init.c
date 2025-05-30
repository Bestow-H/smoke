#include "can.h"
#include "can init.h"
#include "Motor.h"
#include "yaokong.h"
#include "pid.h"
Motor_typedef4005 lk4005;
void CAN_Filter_Init(CAN_HandleTypeDef *hcan)
{
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank           = 0;
    sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh         = 0x0000;
    sFilterConfig.FilterIdLow          = 0x0000;
    sFilterConfig.FilterMaskIdHigh     = 0x0000;
    sFilterConfig.FilterMaskIdLow      = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation     = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) {
        /* Filter configuration Error */
        Error_Handler();
    }
    /* Start the CAN peripheral */
    if (HAL_CAN_Start(hcan) != HAL_OK) {
        /* Start Error */
        Error_Handler();
    }
    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }
    /* Activate CAN TX notification */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }
}

uint8_t CAN1RXmessage[8];

CAN_RxHeaderTypeDef rx1;


Motor_typedef GM6020_rx;
Motor_typedef GM6020_angel;
Motor_typedef GM3508_rx1;
Motor_typedef GM3508_rx2;
Motor_typedef GM3508_rx3;
Motor_typedef GM3508_rx4;
void CAN_Calc(CAN_HandleTypeDef *hcan, uint32_t ID);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{              
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx1, CAN1RXmessage);
	
    CAN_Calc(hcan, rx1.StdId);

	
}
//PIDController_computeinner(&PID1, GM6020_demo.data.Current, 0.1,100, 1, 0.1,100)PIDController_computeouter(&PID, GM6020_demo.data.rawEncode, 0,0, 0, 0,remote_t.control.ch3_int16*6.06+4000)
//PIDController_computeinner(&PID1, GM6020_demo.data.rawSpeed, 0.1,100, 1, 0.1,100)PIDController_computeinner(&PID1,GM6020_demo.data.rawSpeed,0,70, 0, 0,0)
//remote_t.control.ch3_int16*6.06+4000  PIDController_computeinner(&PID1, GM6020_demo.PIDController_computeinner(&PID1,GM6020_demo.data.rawSpeed,0,70, 0, 0,0)ata.rawSpeed*100, 0.05,1.0, 0.1, 0.01,10000)
//jiaodushunghuanPIDController_computeinner(&PID1,GM6020_demo.data.rawSpeed,0.1,70, 1, 0.1,PIDController_computeouter(&PID, GM6020_demo.data.rawEncode, 0.05,0.03, 0.003, 0,remote_t.control.ch3_int16*6.06+4000))
void CAN_Calc(CAN_HandleTypeDef *hcan, uint32_t ID)
{
    if (hcan->Instance == CAN1) {
        
            if (ID==0x202)
							{ MotorResolve(&GM3508_rx1, CAN1RXmessage); }
						if (ID==0x206)
							{ MotorResolve(&GM3508_rx2, CAN1RXmessage); }
            if (ID==0x203)
							{ MotorResolve(&GM3508_rx3, CAN1RXmessage); }
            if (ID==0x204)
							{ MotorResolve(&GM3508_rx4, CAN1RXmessage); }
           if (ID==0x142)
						  {LK_MotorResolve(&lk4005, CAN1RXmessage);}
    }
}
void CAN_send(CAN_HandleTypeDef *_hcan, int16_t stdid, int16_t num1, int16_t num2, int16_t num3, int16_t num4)
{
    CAN_TxHeaderTypeDef tx;
    uint8_t Data[8];
    uint32_t mailbox = 0;
    tx.DLC           = 0x08;
    tx.IDE           = CAN_ID_STD;
    tx.RTR           = CAN_RTR_DATA;
    tx.StdId         = stdid;
    tx.ExtId         = 0x000;
    Data[0]          = ((num1) >> 8);
    Data[1]          = (num1);
    Data[2]          = ((num2) >> 8);
    Data[3]          = (num2);
    Data[4]          = ((num3) >> 8);
    Data[5]          = (num3);
    Data[6]          = ((num4) >> 8);
    Data[7]          = (num4);

    HAL_CAN_AddTxMessage(&hcan, &tx, Data, &mailbox);
}
void LKMF_iq_ctrl(CAN_HandleTypeDef* hcan, uint16_t motor_id, int32_t iqControl)
{
	CAN_TxHeaderTypeDef tx;
	uint32_t mailbox = 0;
	uint8_t data[8];
	uint16_t id =  motor_id;	
	
	tx.StdId = 0x141;
	tx.IDE = CAN_ID_STD;
	tx.RTR = CAN_RTR_DATA;
	tx.DLC = 0x08;
	
	data[0] = 0xA1;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = *(uint8_t *)(&iqControl);
	data[5] = *((uint8_t *)(&iqControl)+1);
	data[6] = 0x00;
	data[7] = 0x00;
	
	 HAL_CAN_AddTxMessage(hcan, &tx, data, &mailbox);
}
void LKMF_Data_Read(CAN_HandleTypeDef* hcan, uint16_t motor_id)
{
	CAN_TxHeaderTypeDef tx;
	uint32_t mailbox = 0;
	uint8_t data[8];	
	
	tx.StdId = 0x140 + motor_id;
	tx.IDE = CAN_ID_STD;
	tx.RTR = CAN_RTR_DATA;
	tx.DLC = 0x08;
	
	data[0] = 0x9c;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	
	 HAL_CAN_AddTxMessage(hcan, &tx, data, &mailbox);
}