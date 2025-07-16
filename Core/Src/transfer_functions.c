/*
 * transfer_functions.c
 *
 *  Created on: Feb 20, 2024
 *      Author: marcelc
 */

#include "transfer_functions.h"
#include "functions.h"
#include "main.h"
#include "config.h"

uint16_t TF_3V3(uint8_t bytes, uint32_t raw, Sensor *sensor){


	uint16_t max_volt = 3300;

	uint16_t voltage = raw*max_volt / 4095;
	return voltage;
}

uint16_t TF_5V(uint8_t bytes, uint32_t raw, Sensor *sensor){
	uint16_t max_volt = 5000;

	uint16_t voltage = raw*max_volt / 4095;
	return voltage;

}

uint16_t TF_24V(uint8_t bytes, uint32_t raw, Sensor *sensor){
	uint16_t max_volt = 24000;

	uint16_t voltage = raw*max_volt / 4095;
	return voltage;
}

uint16_t bpps_global;
uint16_t TF_BPPS(uint8_t bytes, uint32_t raw, Sensor *sensor){ //brake pedal position sensor
	uint16_t max_pos = 100; //in percent

	uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 2615 ;
	uint16_t max_raw = (sensor->calib_code > 2 && sensor->high_adc != 0) ? sensor->high_adc : 2925;

	uint16_t bpps = (raw-min_raw)*max_pos / (max_raw-min_raw);

	if(min_raw > max_raw){

		bpps = 100 - bpps;
		bpps_global = bpps;

	}
	return bpps;

}
uint16_t TF_APPS1(uint8_t bytes, uint32_t raw, Sensor *sensor){
	uint16_t max_pos = 100; //in percent

	uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 650;
	uint16_t max_raw = (sensor->calib_code > 2 &&  sensor->high_adc != 0) ? sensor->high_adc : 1990 ;

	uint16_t apps = (raw-min_raw)*max_pos / (max_raw-min_raw);

	if(min_raw > max_raw){

		apps = 100 - apps;
	}
	return apps;
}

uint16_t TF_APPS2(uint8_t bytes, uint32_t raw, Sensor *sensor){
	uint16_t max_pos = 100; //in percent

	uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 690 ;
	uint16_t max_raw = (sensor->calib_code >= 2 && sensor->high_adc != 0) ? sensor->high_adc : 2830;

	uint16_t apps = (raw-min_raw)*max_pos / (max_raw-min_raw);

	if(min_raw > max_raw){

		apps = 100 - apps;
	}
	return apps;
}

uint16_t TF_BTN(uint8_t bytes, uint32_t raw, Sensor *sensor){
		uint16_t max_volt = 5000;

		uint16_t voltage = raw*max_volt / 4095;

		if(voltage < 2000)
			return 0;
		else
			return 1;
}

uint16_t TF_FRONT_HEAVE(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;

}
uint16_t TF_REAR_HEAVE(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}
uint16_t TF_FRONT_ROLL(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}
uint16_t TF_REAR_ROLL(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}
uint16_t TF_TYRE_TEMP(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}
uint16_t TF_ANGLE_GEAR(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}
uint16_t TF_WATER_LVL(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}

uint16_t TF_BRK_PRES(uint8_t bytes, uint32_t raw, Sensor *sensor){
	return 0;
}
