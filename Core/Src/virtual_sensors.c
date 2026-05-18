#include "virtual_sensors.h"
#include "config.h"
#define APPS_DELTA_MAX_X10 100
#define PEDALREQ_FAULT_NONE 0
#define PEDALREQ_FAULT_APPS_DELTA 1

// TODO: move majority of virtual sensor code here
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
