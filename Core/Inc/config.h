#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "main.h"
#define CHANGE_SENSOR_CONFIG 0x00AA

#define SENSOR_NUM 12 // MAX number of sensors handled by the board.
typedef struct SensorType Sensor;
#define MAX_INPUT_SENSORS 12
typedef struct VirtualSensor VirtualSensor;

typedef struct
{
  uint32_t values[3];
} SensorData;

typedef struct
{
  Sensor* apps1;
  Sensor* apps2;
} PedalContext;

void Config_Setup(void);
void Change_Sensor_Config(uint8_t opt);

typedef volatile struct CanDataType CANData;

typedef uint16_t (*DataHandler)(uint8_t bytes, uint32_t raw, Sensor* sensor);

struct SensorType
{
  DataHandler transfer_function;
  uint16_t CAN_ID;
  uint16_t CAN_interval;
  uint32_t averages;
  uint8_t pin;
  uint8_t calib_code;
  uint16_t data;
  uint16_t low_adc;
  uint16_t high_adc;
};

void check_calib_status(Sensor* sensor);
void read_all_calib_values();
void ADC_Calib_Update();

// Ensure external visibility of the variables
extern Sensor sensors[SENSOR_NUM];
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
