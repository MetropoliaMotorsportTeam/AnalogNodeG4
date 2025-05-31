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

#define virt_sensor_can_id_1 17;
#define virt_sensor_can_id_2 18;
#define virt_sensor_can_id_3 19;
#define virt_sensor_can_id_4 20;
#define virt_sensor_can_id_5 21;
#define virt_sensor_can_id_6 22;
#define virt_sensor_can_id_7 23;
#define virt_sensor_can_id_8 24;
#define virt_sensor_can_id_9 25;
#define virt_sensor_can_id_10 26;
#define virt_sensor_can_id_11 27;
#define virt_sensor_can_id_12 28;
#define virt_sensor_can_id_13 29;
#define virt_sensor_can_id_14 30;

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

#endif /* INC_FUNCTIONS_H_ */
