#include "config.h"

#define FLASH_ADDRESS 0x0801F800

#define ID 1


void Config_Setup(void) {
#if ID == 1
    Config_1();
#elif ID == 2
    Config_2();
#elif ID == 3
    Config_3();
#endif
}


void  ADC_Update(){
	FLASH_EraseInitTypeDef FlashErase;
	uint32_t PageError = 0;

	HAL_FLASH_Unlock();

	for(int i = 0; i < 12; i++){

		FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;
		FlashErase.Page = FLASH_ADDRESS + i * 4; //4 is the length of 1 word
		FlashErase.NbPages = 1;

		 // Perform the erase operation
		if (HAL_FLASHEx_Erase(&FlashErase, &PageError) != HAL_OK) {
				// Handle error if the erase operation fails
				// Use PageError to check which page caused the issue
		}

		uint16_t data_to_write = (uint32_t(sensors[i].high_adc) << 16) | sensors[i].low_adc;

		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST, FLASH_ADDRESS + i * 4, data_to_write) != HAL_OK) {
			// Handle error
		}
		//TODO add check up that data is saved correctly
	}
	    // Lock the Flash memory after operation
	 HAL_FLASH_Lock();


}

void read_all_calib_values(){

	for(int i = 0; i < 12; i++){

		uint32_t value = *(__IO uint32_t*)FLASH_ADDRESS + i * 4;

		sensors[i].low_adc = value;
		sensors[i].high_adc = value >> 16;
	}
}


void Config_1(void) {

	//initialize the sensors
	for (int i = 0; i < 16; i++) {
		sensors[i].transfer_function = TF_3V3;
		sensors[i].CAN_ID = 0;
		sensors[i].CAN_interval = 20;
		sensors[i].averages = 0;
		sensors[i].pin = i;
	}

	// Sensor definitions
	Sensor BTN1 = {TF_BTN, 12, 100, 0, V5_in0};
	Sensor BTN2 = {TF_BTN, 13, 100, 0, V5_in5};
	Sensor BTN3 = {TF_BTN, 14, 100, 0, V5_in4};
	Sensor APPS2 = {TF_APPS2, 2, 100, 0, V5_in1};
	Sensor APPS1 = {TF_APPS1, 1, 100, 0, V24_in1};
	Sensor ANGLE = {TF_24V, 6, 100, 0, V24_in0};
	Sensor BPPS = {TF_BPPS, 3, 100, 0, V5_in2};
	Sensor F_ROLL = {TF_5V, 10, 100, 0, V5_in6};
	Sensor F_HEAVE = {TF_5V, 8, 100, 0, V5_in3};

    sensors[APPS1.pin] = APPS1;
    sensors[APPS2.pin] = APPS2;
    sensors[BPPS.pin] = BPPS;
	sensors[BTN1.pin] = BTN1;
	sensors[BTN2.pin] = BTN2;
	sensors[BTN3.pin] = BTN3;
	sensors[ANGLE.pin] = ANGLE;
	sensors[F_ROLL.pin] = F_ROLL;
	sensors[F_HEAVE.pin] = F_HEAVE;
	

    CAN_interval = 20;
}

void Config_2(void){
	//initialize the sensors
		for (int i = 0; i < 16; i++) {
			sensors[i].transfer_function = TF_3V3;
			sensors[i].CAN_ID = 0;
			sensors[i].CAN_interval = 100;
			sensors[i].averages = 0;
			sensors[i].pin = i;
		}

		// Sensor definitions

	    CAN_interval = 100;
}
