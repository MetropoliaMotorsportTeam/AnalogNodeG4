#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "main.h"
#include "transfer_functions.h"

#define SENSOR_NUM 12 //MAX number of sensors handled by the board.

// Function Prototypes
void Config_Setup(void);
void Config_1(void);
void Config_2(void);
void Config_3(void);

typedef volatile struct CanDataType CANData;

typedef uint16_t (*DataHandler)(uint8_t bytes, uint32_t raw, CANData *sensor);

typedef struct SensorType {
    DataHandler transfer_function;
    uint16_t CAN_ID;
    uint16_t CAN_interval;
    uint32_t averages;
    uint8_t pin;
    uint8_t calib_code;
    uint16_t data;
    uint16_t low_adc;
    uint16_t high_adc;
} Sensor;

void check_calib_status(Sensor *sensor);
void read_all_calib_values();
void ADC_Calib_Update();


// Ensure external visibility of the variables
extern Sensor sensors[SENSOR_NUM];
extern uint16_t CAN_interval;

// Enum for Pins
enum Pins {
	V3_in0, V3_in1, V5_in0, V5_in1, V24_in2, V24_in1, V5_in4,
	    V5_in5, V5_in6, V24_in0, V5_in3, V5_in2
};

#endif /* INC_CONFIG_H_ */
