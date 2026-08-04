#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <stdint.h>
#define CAN_CHANGE_CONFIG 0x10
#define DEFAULT_CONF 2
#define NUM_CONF 4

void Config_Setup(void);
void apply_config(uint8_t config);
void load_config(void);
void process_config(uint8_t config);

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

#define APPS1_ID 0x01
#define APPS2_ID 0x02
#define BPPS_ID 0x03

#define BrakeFront_ID 0x04
#define BrakeRear_ID 0x05
#define SteeringAngle_ID 0x06
#define WaterLevel_ID 0x07
#define HeavesFront_ID 0x08
#define HeavesRear_ID 0x09
#define Rolls1_ID 0x0A
#define Rolls2_ID 0x0B

#define BTN1_ID 0x0C
#define BTN2_ID 0x0D
#define BTN3_ID 0x0E

#define CONFIG_FLASH_ADDR 0x0801F000U
#define CONFIG_FLASH_PAGE ((CONFIG_FLASH_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE)
#define CONFIG_SLOT_SIZE 8
#define CONFIG_NUM_SLOTS (FLASH_PAGE_SIZE) / (CONFIG_SLOT_SIZE)

#endif /* INC_CONFIG_H_ */
