#include "pedal_map.h"
#include "sensors.h"
#include "virtual_sensors.h"
#include <string.h>

const uint16_t* curr_curve = pedal_curve_aggressive;
static uint16_t pedal_curve_slots[PEDAL_CONFIG_SLOTS][PEDAL_LUT_SIZE];

// test function
uint16_t pedal_map_get_percentage(const uint16_t* curve)
{
#ifdef VIRTUAL_SENSOR
  pedalreq.update_func(&pedalreq);
  return pedal_map(pedalreq.output.values[0], curve);
#else
  return pedal_map(APPS_pedal->transfer_function(1, APPS_pedal->averages, APPS_pedal));
#endif
  return 0;
}

uint16_t pedal_map(uint16_t pedal)
{

  if (pedal >= PEDAL_MAX_VALUE)
  {
    return PEDAL_MAX_VALUE;
  }

  uint16_t index = pedal / PEDAL_LUT_STEP;
  uint16_t remainder = pedal % PEDAL_LUT_STEP;

  uint16_t y0 = curr_curve[index];
  uint16_t y1 = curr_curve[index + 1];

  uint32_t delta = (uint32_t)(y1 - y0);
  uint32_t y = (uint32_t)y0 + ((delta * remainder) / PEDAL_LUT_STEP);

  if (y > PEDAL_MAX_VALUE)
  {
    y = PEDAL_MAX_VALUE;
  }

  return (uint16_t)y;
}

void change_pedal_curve(pedal_curve curve)
{
  // TODO: rewrite to be able to switch to non-preset curve
  switch (curve)
  {
  case LINEAR:
    curr_curve = pedal_curve_linear;
    break;

  case PARABOLIC:
    curr_curve = pedal_curve_aggressive;
    break;

  case SOFT:
    curr_curve = pedal_curve_soft;
    break;

  case STUPID:
    curr_curve = pedal_curve_stupid;
    break;

  default:
    break;
  }
}

void process_pedal_config(CAN_Message msg)
{
  static uint8_t CAN_msg_buf[PEDAL_LUT_SIZE];
  static uint8_t slot = 0;
  static uint8_t size = 0;

  // TODO:make documents for what data needs to be sent
  if (size == PEDAL_LUT_SIZE)
  {
    add_pedal_curve(CAN_msg_buf, slot, size);
    memset(CAN_msg_buf, 0, sizeof(CAN_msg_buf));
    slot = 0;
    size = 0;
  }
}

uint8_t add_pedal_curve(uint8_t values[], uint8_t slot, uint8_t size)
{
  if (size < PEDAL_LUT_SIZE)
    return 1;

  if (!values)
    return 1;

  memcpy(pedal_curve_slots[slot], values, PEDAL_LUT_SIZE);
  return 0;
}
