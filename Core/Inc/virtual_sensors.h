#ifndef VIRTUAL_SENSORS_H
#define VIRTUAL_SENSORS_H

#include "config.h"
#include "sensors.h"

#define APPS_DELTA_MAX 100
#define PEDALREQ_FAULT_NONE 0
#define PEDALREQ_FAULT_APPS_DELTA 1

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

extern VirtualSensor pedalreq;

void update_pedalreq(VirtualSensor* v_sensor);
void init_virtual_sensor(VirtualSensor* v_sensor, const char* name, SensorUpdateFunc func,
                         void* context, uint16_t CAN_ID);
void add_input_sensor(VirtualSensor* v_sensor, Sensor* input);
void update_virtual_sensor(VirtualSensor* v_sensor);

#endif
