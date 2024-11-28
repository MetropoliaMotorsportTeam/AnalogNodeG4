/*
 * functions.c
 *
 *  Created on: Mar 4, 2024
 *      Author: csort
 */

#include "transfer_functions.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"
#include "config.h"
#include "main.h"

typedef struct{
	uint8_t DLC;
	uint8_t Bytes[8];
}CAN_Message;

CAN_Message RxMessage;
CAN_Message TxMessage;

void CanSend(uint8_t *TxData){
	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) != 0 && HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK){
		Error_Handler();
	}
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxMessage.Bytes) != HAL_OK)
		{
			/* Reception Error */
			Error_Handler();
		}else{
			decode();
		}

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
		{
			/* Notification Error */
			Error_Handler();
		}
	}
}



void print(uint16_t select){

		/*
		sensors[select].data = sensors[select].transfer_function(1,sensors[select].averages);
		TxMessage.Bytes[0] = sensors[select].data;
		TxMessage.Bytes[1] = sensors[select].data >> 8;
		TxHeader.Identifier = sensors[select].CAN_ID;

		if(sensors[select].CAN_ID)
			CanSend(TxMessage.Bytes);
		*/

	CAN_Message Tx;
	Tx.DLC = 8;
	Tx.Bytes[0] = 0x01;
	Tx.Bytes[1] = 0x02;
	Tx.Bytes[2] = 0x03;
	Tx.Bytes[3] = 0x04;
	Tx.Bytes[4] = 0x05;
	Tx.Bytes[5] = 0x06;
	Tx.Bytes[6] = 0x07;
	Tx.Bytes[7] = 0x08;

	TxHeader.Identifier = 0x123;

	CanSend(Tx.Bytes);

			
}

void decode(){
	switch(RxMessage.Bytes[0]){
	
	}
}
