/*
 * transfer_functions.h
 *
 *  Created on: Feb 20, 2024
 *      Author: marcelc
 */

#ifndef INC_TRANSFER_FUNCTIONS_H_
#define INC_TRANSFER_FUNCTIONS_H_

#include "main.h"
#include "config.h"

#endif /* INC_TRANSFER_FUNCTIONS_H_ */


uint16_t TF_3V3(uint8_t bytes, uint32_t raw);
uint16_t TF_5V(uint8_t bytes, uint32_t raw);
uint16_t TF_24V(uint8_t bytes, uint32_t raw);
uint16_t TF_BPPS(uint8_t bytes, uint32_t raw);
uint16_t TF_APPS1(uint8_t bytes, uint32_t raw);
uint16_t TF_APPS2(uint8_t bytes, uint32_t raw);
uint16_t TF_FRONT_HEAVE(uint8_t bytes, uint32_t raw);
uint16_t TF_REAR_HEAVE(uint8_t bytes, uint32_t raw);
uint16_t TF_FRONT_ROLL(uint8_t bytes, uint32_t raw);
uint16_t TF_REAR_ROLL(uint8_t bytes, uint32_t raw);
uint16_t TF_TYRE_TEMP(uint8_t bytes, uint32_t raw);
uint16_t TF_ANGLE_GEAR(uint8_t bytes, uint32_t raw);
uint16_t TF_WATER_LVL(uint8_t bytes, uint32_t raw);
uint16_t TF_BRK_PRES(uint8_t bytes, uint32_t raw);
uint16_t TF_BTN(uint8_t bytes, uint32_t raw);
