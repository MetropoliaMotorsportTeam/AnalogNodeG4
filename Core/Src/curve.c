#include "curve.h"
#include "sensors.h"
#include "virtual_sensors.h"

uint16_t pedal_map(uint16_t pedal, const uint16_t* curve)
{
  if (pedal >= PEDAL_MAX_VALUE)
  {
    return PEDAL_MAX_VALUE;
  }

  uint16_t index = pedal / PEDAL_LUT_STEP;
  uint16_t remainder = pedal % PEDAL_LUT_STEP;

  uint16_t y0 = curve[index];
  uint16_t y1 = curve[index + 1];

  uint32_t delta = (uint32_t)(y1 - y0);
  uint32_t y = (uint32_t)y0 + ((delta * remainder) / PEDAL_LUT_STEP);

  if (y > PEDAL_MAX_VALUE)
  {
    y = PEDAL_MAX_VALUE;
  }

  return (uint16_t)y;
}
