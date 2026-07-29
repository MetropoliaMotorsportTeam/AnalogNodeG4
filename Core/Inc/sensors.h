#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

#define MAX_INPUT_SENSORS 12
#define SENSOR_NUM 12
#define ADC_CALIB_FLASH_ADDR 0x0801E800U
#define ADC_CALIB_FLASH_PAGE ((ADC_CALIB_FLASH_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE)

typedef struct SensorType Sensor;
typedef int16_t (*DataHandler)(uint8_t bytes, uint32_t raw, Sensor* sensor);

struct SensorType
{
  DataHandler transfer_function;
  uint16_t CAN_ID;
  uint16_t CAN_interval;
  uint32_t averages;
  uint8_t pin;
  uint8_t calib_code;
  int16_t data;
  uint16_t low_adc;
  uint16_t high_adc;
};
void check_calib_status(Sensor* sensor);
void read_all_calib_values();
void ADC_Calib_Update();
void init_sensors(void);

extern Sensor sensors[SENSOR_NUM];
extern Sensor* APPS1Pedal;
extern Sensor* APPS2Pedal;
extern Sensor* BPPSPedal;

#endif
