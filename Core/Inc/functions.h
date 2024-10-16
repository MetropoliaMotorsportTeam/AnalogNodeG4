/*
 * functions.h
 *
 *  Created on: Mar 4, 2024
 *      Author: csort
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#define CAN_CALIB_ID 31
#define CAN_CALIIB_DONE_ID 32

typedef struct{
	uint16_t Id;
	uint8_t DLC;
	uint8_t Bytes[8];
}CAN_Message;

extern uint8_t sensor_for_calib; //Sensor calibration number
extern int8_t calib_select; //Upper or lower calibration

void CanSend(uint8_t *TxData);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void print(uint16_t select);
void decode(CAN_Message msg);
void calibration();

//extern CAN_Message RxMessage;
//extern CAN_Message TxMessage;

#endif /* INC_FUNCTIONS_H_ */
