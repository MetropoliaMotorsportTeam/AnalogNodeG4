#include "virtual_sensors.h"
#include "config.h"
#include "sensors.h"
#include <string.h>
#define APPS_DELTA_MAX_X10 100
#define PEDALREQ_FAULT_NONE 0
#define PEDALREQ_FAULT_APPS_DELTA 1

void update_pedalreq(VirtualSensor* v_sensor)
{
  if (v_sensor->input_count < 2)
  {
    v_sensor->output.values[0] = 0;
    v_sensor->output.values[1] = PEDALREQ_FAULT_APPS_DELTA;
    return;
  }

  Sensor* apps1_sensor = v_sensor->inputs[0];
  Sensor* apps2_sensor = v_sensor->inputs[1];

  uint16_t apps1 = apps1_sensor->data;
  uint16_t apps2 = apps2_sensor->data;

  uint16_t delta = apps1 > apps2 ? apps1 - apps2 : apps2 - apps1;

  if (delta > APPS_DELTA_MAX_X10)
  {
    v_sensor->output.values[0] = 0;
    v_sensor->output.values[1] = PEDALREQ_FAULT_APPS_DELTA;
    v_sensor->output.values[2] = delta;
    return;
  }

  uint16_t request = (apps1 + apps2) / 2;

  v_sensor->output.values[0] = request;
  v_sensor->output.values[1] = PEDALREQ_FAULT_NONE;
  v_sensor->output.values[2] = delta;
}

void init_virtual_sensor(VirtualSensor* v_sensor, const char* name, SensorUpdateFunc func,
                         void* context, uint16_t CAN_ID)
{
  v_sensor->name = name;
  v_sensor->update_func = func;
  v_sensor->context = context;
  v_sensor->CAN_ID = CAN_ID;
  v_sensor->input_count = 0;
  memset(&v_sensor->output, 0, sizeof(SensorData));
}

void add_input_sensor(VirtualSensor* v_sensor, Sensor* input)
{
  if (v_sensor->input_count < MAX_INPUT_SENSORS)
  {
    v_sensor->inputs[v_sensor->input_count++] = input;
  }
}

void update_virtual_sensor(VirtualSensor* v_sensor)
{
  if (v_sensor->update_func)
  {
    v_sensor->update_func(v_sensor->context);
  }
}
