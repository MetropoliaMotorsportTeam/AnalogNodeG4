#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "main.h"
#include "transfer_functions.h"

// Function Prototypes
void Config_Setup(void);
void Config_1(void);
void Config_2(void);
void Config_3(void);

typedef uint16_t (*DataHandler)(uint8_t bytes, uint32_t raw);

typedef struct {
    DataHandler transfer_function;
    uint16_t CAN_ID;
    uint16_t CAN_interval;
    uint32_t averages;
    uint8_t pin;
    uint16_t data;
} Sensor;

// Ensure external visibility of the variables
extern Sensor sensors[16];
extern uint16_t CAN_interval;

// Enum for Pins
enum Pins {
    V24_in1, V3_in0, V3_in1, V24_in2, NC, NC1, V24_in0,
    V5_in6, V5_in5, V5_in4, V5_in1, V5_in0, V5_in3, V5_in2, NC2, NC3
};


#endif /* INC_CONFIG_H_ */
