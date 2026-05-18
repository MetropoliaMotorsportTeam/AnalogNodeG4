#ifndef VIRTUAL_SENSORS_H
#define VIRTUAL_SENSORS_H

#include "config.h"
typedef struct VirtualSensor VirtualSensor;
typedef void (*SensorUpdateFunc)(VirtualSensor* v_sensors);

struct VirtualSensor
{
  const char* name;
  SensorData output;
  uint16_t CAN_ID;
  void* context;
  SensorUpdateFunc update_func;
  Sensor* inputs[MAX_INPUT_SENSORS];
  uint32_t input_count;
};
void update_pedalreq(VirtualSensor* v_sensor);
#endif
