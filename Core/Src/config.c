#include "config.h"

#define FLASH_ADDRESS 0x0801F800

#define ID 1


void Config_Setup(void) {

	read_all_calib_values();

#if ID == 1
    Config_1();
#elif ID == 2
    Config_2();
#elif ID == 3
    Config_3();
#endif
}


void ADC_Update() { //writes all 12 * 2 calibration values into the FLASH memory
    static FLASH_EraseInitTypeDef FlashErase;
    uint32_t PageError = 0;

    // Unlock the Flash memory
    HAL_FLASH_Unlock();

    //Erase memory before writing
    // Configure the flash erase parameters
    FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;
    FlashErase.Page = (FLASH_ADDRESS / FLASH_PAGE_SIZE); // Convert address to page number
    FlashErase.NbPages = 1;

    __disable_irq();

    // Perform the erase operation
    if (HAL_FLASHEx_Erase(&FlashErase, &PageError) != HAL_OK) {
        // Handle the error
        Error_Handler();
    }

    for (int i = 0; i < SENSOR_NUM; i++) {
        // Prepare data to write
        uint64_t data_to_write = (sensors[i].high_adc << 16) | sensors[i].low_adc;
        // Program the flash memory
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_ADDRESS + i * 8, data_to_write) != HAL_OK) { //TODO Figure out how to write just 32 bits without a need for filler 0s
            Error_Handler();
        }
    }
    // Lock the Flash memory after operation
    HAL_FLASH_Lock();
    // Enable interrupts after the operation
    __enable_irq();
}
void check_calib_status(Sensor *sensor){

	uint16_t default_values = 0xFFFF;
	int8_t code = 3;
	if(sensor->low_adc == default_values )
		code -= 1;
	if(sensor->high_adc == default_values )
		code -= 2;

	sensor->calib_code = code;

}

void read_all_calib_values(){

	for(int i = 0; i < SENSOR_NUM; i++){

		uint32_t value = *(__IO uint32_t*)(FLASH_ADDRESS + i * 8);

		uint16_t low = value;
		uint16_t high = value >> 16;

		sensors[i].low_adc = low;
		sensors[i].high_adc = high;

		check_calib_status(&sensors[i]);
	}
}


void Config_1(void) {

	//initialize the sensors
	for (int i = 0; i < SENSOR_NUM; i++) {
		sensors[i].transfer_function = TF_3V3;
		sensors[i].CAN_ID = 0;
		sensors[i].CAN_interval = 20;
		sensors[i].averages = 0;
		sensors[i].pin = i;
		sensors[i].high_adc = 0xFFFF;
		sensors[i].low_adc = 0xFFFF;
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
		for (int i = 0; i < SENSOR_NUM; i++) {
			sensors[i].transfer_function = TF_3V3;
			sensors[i].CAN_ID = 0;
			sensors[i].CAN_interval = 100;
			sensors[i].averages = 0;
			sensors[i].pin = i;
		}

		// Sensor definitions

	    CAN_interval = 100;
}
