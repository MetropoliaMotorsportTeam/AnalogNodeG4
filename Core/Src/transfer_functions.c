/*
 * transfer_functions.c
 *
 *  Created on: Feb 20, 2024
 *      Author: marcelc
 */

#include "transfer_functions.h"
#include "config.h"
#include "functions.h"
#include "main.h"
#include <math.h>

extern uint16_t v5_line_mv;

uint32_t ValueControl(uint32_t raw, uint32_t min_raw, uint32_t max_raw)
{

  if (raw > max_raw)
    return max_raw;

  if (raw < min_raw)
    return min_raw;

  return raw;
}

uint16_t TF_3V3(uint8_t bytes, uint32_t raw, Sensor* sensor)
{

  uint16_t max_volt = 3300;

  uint16_t voltage = raw * max_volt / 4095;
  return voltage;
}

uint16_t TF_5V(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  uint16_t max_volt = 5000;

  uint16_t voltage = raw * max_volt / 4095;
  return voltage;
}

uint16_t TF_24V(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  uint16_t max_volt = 24000;

  uint16_t voltage = raw * max_volt / 4095;
  return voltage;
}

uint16_t bpps_global;
uint16_t TF_BPPS(uint8_t bytes, uint32_t raw, Sensor* sensor)
{                         // brake pedal position sensor
  uint16_t max_pos = 100; // in percent

  uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 2615;
  uint16_t max_raw = (sensor->calib_code > 2 && sensor->high_adc != 0) ? sensor->high_adc : 2925;

  raw = ValueControl(raw, min_raw, max_raw);

  uint16_t bpps = (raw - min_raw) * max_pos / (max_raw - min_raw);

  if (min_raw > max_raw)
  {

    bpps = 100 - bpps;
    bpps_global = bpps;
  }
  return bpps;
}
uint16_t TF_APPS1(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  uint16_t max_pos = 100; // in percent

  uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 650;
  uint16_t max_raw = (sensor->calib_code > 2 && sensor->high_adc != 0) ? sensor->high_adc : 1990;

  raw = ValueControl(raw, min_raw, max_raw);

  uint16_t apps = (raw - min_raw) * max_pos / (max_raw - min_raw);

  if (min_raw > max_raw)
  {

    apps = 100 - apps;
  }
  return apps;
}

uint16_t TF_APPS2(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  uint16_t max_pos = 100; // in percent

  uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 690;
  uint16_t max_raw = (sensor->calib_code >= 2 && sensor->high_adc != 0) ? sensor->high_adc : 2830;

  raw = ValueControl(raw, min_raw, max_raw);

  uint16_t apps = (raw - min_raw) * max_pos / (max_raw - min_raw);

  if (min_raw > max_raw)
  {

    apps = 100 - apps;
  }
  return apps;
}

uint16_t TF_BTN(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  uint16_t max_volt = 5000;

  uint16_t voltage = raw * max_volt / 4095;

  if (voltage < 2000)
    return 0;
  else
    return 1;
}

uint16_t TF_FRONT_HEAVE(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}
uint16_t TF_REAR_HEAVE(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}
uint16_t TF_FRONT_ROLL(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}
uint16_t TF_REAR_ROLL(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}
uint16_t TF_TYRE_TEMP(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}
uint16_t TF_ANGLE_GEAR(uint8_t bytes, uint32_t raw, Sensor* sensor)
{

  float SteeringAngleScope = 320; // how many degrees of movement steering wheel can do
  float WheelAngleScope = 40;
  uint16_t min_raw = (sensor->calib_code % 2 == 1 && sensor->low_adc != 0) ? sensor->low_adc : 690;
  uint16_t max_raw = (sensor->calib_code >= 2 && sensor->high_adc != 0) ? sensor->high_adc : 2830;

  raw = ValueControl(raw, min_raw, max_raw);

  float SteeringAngle = (raw - min_raw) * WheelAngleScope / (max_raw - min_raw);
  SteeringAngle -= SteeringAngleScope / 2;

  return SteeringAngle;
}
uint16_t TF_WATER_LVL(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}

uint16_t TF_BRK_PRES(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}

uint16_t TF_5V_ASSIGN(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  (void)bytes;

  uint16_t max_volt = 5000;
  uint16_t voltage = raw * max_volt / 4095;

  v5_line_mv = voltage;

  return voltage;
}

uint16_t TF_WATER_TEMP(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  (void)bytes;
  (void)sensor;

  const float B = 3988.0f;
  const float R0 = 10000.0f;
  const float T0 = 298.15f;
  const float R1 = 10000.0f;

  // TODO: replace this with measured 5V rail
  float Vs = (v5_line_mv > 0) ? v5_line_mv : 5000.0f;

  const float Vref = 3300.0f;

  float v = ((float)raw / 4095.0f) * Vref;

  if (v <= 0.0f)
    v = 0.001f;

  if (v >= Vs)
    v = Vs - 0.001f;

  float r = (v * R1) / (Vs - v);

  float temp_kelvin = 1.0f / ((1.0f / T0) + (logf(r / R0) / B));
  float temp_celsius = temp_kelvin - 273.15f;

  return (uint16_t)temp_celsius;
}
