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
#include "functions.h"

CAN_Message RxMessage;
CAN_Message TxMessage;

uint8_t sensor_for_calib; //Sensor calibration number
int8_t calib_select = -1; //Upper or lower calibration

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

			RxMessage.Id = RxHeader.Identifier;
			RxMessage.DLC = RxHeader.DataLength;

			decode(RxMessage);
		}

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
		{
			/* Notification Error */
			Error_Handler();
		}
	}
}



void print(uint16_t select){
		//uint16_t Data = TF_Select(1,averages[select],transfer_functions[select]);
		sensors[select].data = sensors[select].transfer_function(1,sensors[select].averages, &sensors[select]);
		TxMessage.Bytes[0] = sensors[select].data;
		TxMessage.Bytes[1] = sensors[select].data >> 8;
		TxHeader.Identifier = sensors[select].CAN_ID;

		if(sensors[select].CAN_ID)
			CanSend(TxMessage.Bytes);
}


void sent_calib_done(){

	TxHeader.Identifier = CAN_CALIIB_DONE_ID;
	TxMessage.Bytes[0] = sensor_for_calib;
	CanSend(TxMessage.Bytes);


	sensors[sensor_for_calib].calib_code = sensors[sensor_for_calib].calib_code | (1 << calib_select);


}

uint8_t calibration_counter = 0;
uint32_t calibration_value = 0;
void calibration(){

	if(calib_select == -1){
		return;
	}
	else{
		calibration_value += sensors[sensor_for_calib].averages;
		calibration_counter++;

		if(calibration_counter > 3000 / CAN_interval){

			calibration_value = calibration_value / calibration_counter;

			if(calib_select == 0)
				sensors[sensor_for_calib].low_adc = calibration_value;
			if(calib_select == 1)
				sensors[sensor_for_calib].high_adc = calibration_value;


			calib_select = -1;
			calibration_counter = 0;
		}

	}
}

void decode(CAN_Message msg){
	switch(msg.Id){
		case CAN_CALIB_ID:
			sensor_for_calib = msg.Bytes[0];
			calib_select = msg.Bytes[1];
			break;
	}
}
