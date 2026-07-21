#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "main.h"
#define CAN_CHANGE_CONFIG 0x10
#define FLASH_ADDRESS 0x0801F800
#define DEFAULT_CONF 2
#define NUM_CONF 4

void Config_Setup(void);
void apply_config(uint8_t config);
void load_config(void);
void Process_Config(uint8_t config);

typedef volatile struct CanDataType CANData;

// Ensure external visibility of the variables
extern uint16_t CAN_interval;

// Enum for Pins
enum Pins
{
  V3_in0,
  V3_in1,
  V5_in0,
  V5_in1,
  V24_in2,
  V24_in1,
  V5_in4,
  V5_in5,
  V5_in6,
  V24_in0,
  V5_in3,
  V5_in2
};

#define V5_LINE_PIN V5_in1

#endif /* INC_CONFIG_H_ */
