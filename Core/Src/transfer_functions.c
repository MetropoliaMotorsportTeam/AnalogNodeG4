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
#include "sensors.h"
#include <math.h>

static uint16_t bpps_global = 0;

static inline void get_calib_values(Sensor* sensor, uint16_t* low, uint16_t* high)
{
  if ((sensor->calib_code & CALIB_LOW_VALID) && sensor->low_adc != CALIB_DEFAULT)
  {
    *low = sensor->low_adc;
  }

  if ((sensor->calib_code & CALIB_HIGH_VALID) && sensor->high_adc != CALIB_DEFAULT)
  {
    *high = sensor->high_adc;
  }
}

static inline uint32_t ValueControl(uint32_t raw, uint32_t min_raw, uint32_t max_raw)
{
  if (raw > max_raw)
    return max_raw;
  if (raw < min_raw)
    return min_raw;
  return raw;
}

uint16_t TF_3V3(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  const uint16_t max_volt = 3300;

  uint16_t voltage = raw * max_volt / 4095;
  sensor->data = voltage;
  return voltage;
}

uint16_t TF_5V(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  const uint16_t max_volt = 5000;

  uint16_t voltage = raw * max_volt / 4095;
  sensor->data = voltage;
  return voltage;
}

uint16_t TF_24V(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  uint16_t max_volt = 24000;

  uint16_t voltage = raw * max_volt / 4095;
  sensor->data = voltage;
  return voltage;
}

uint16_t TF_BPPS(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  const uint16_t max_pos = 100;

  uint16_t min_raw = 2615;
  uint16_t max_raw = 2925;

  get_calib_values(sensor, &min_raw, &max_raw);

  raw = ValueControl(raw, min_raw, max_raw);

  uint16_t bpps = (raw - min_raw) * max_pos / (max_raw - min_raw);

  if (min_raw > max_raw)
  {
    bpps = 100 - bpps;
  }

  bpps_global = bpps;
  sensor->data = bpps;
  return bpps;
}

uint16_t TF_APPS1(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  const uint16_t max_pos = 1000;

  uint16_t min_raw = 1105;
  uint16_t max_raw = 1940;

  get_calib_values(sensor, &min_raw, &max_raw);

  raw = ValueControl(raw, min_raw, max_raw);

  uint16_t range = max_raw - min_raw;
  uint16_t apps = ((raw - min_raw) * max_pos + (range / 2)) / range;

  if (min_raw > max_raw)
  {
    apps = 1000 - apps;
  }

  sensor->data = apps;
  return apps;
}

uint16_t TF_APPS2(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  const uint16_t max_pos = 1000;

  uint16_t min_raw = 690;
  uint16_t max_raw = 2830;

  get_calib_values(sensor, &min_raw, &max_raw);

  raw = ValueControl(raw, min_raw, max_raw);

  uint16_t range = max_raw - min_raw;
  uint16_t apps = ((raw - min_raw) * max_pos + (range / 2)) / range;

  if (min_raw > max_raw)
  {
    apps = 1000 - apps;
  }

  sensor->data = apps;
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
  float SteeringAngleScope = 320;
  float WheelAngleScope = 40;

  uint16_t min_raw = 690;
  uint16_t max_raw = 2830;

  get_calib_values(sensor, &min_raw, &max_raw);

  raw = ValueControl(raw, min_raw, max_raw);

  float SteeringAngle = (raw - min_raw) * WheelAngleScope / (max_raw - min_raw);
  SteeringAngle -= SteeringAngleScope / 2;

  sensor->data = (uint16_t)SteeringAngle;
  return (uint16_t)SteeringAngle;
}

uint16_t TF_WATER_LVL(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}

uint16_t TF_BRK_PRES(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  return 0;
}

static inline uint16_t GET_5V_LINE(enum Pins pin)
{
  return sensors[pin].transfer_function(1, sensors[pin].averages, &sensors[pin]);
}

uint16_t TF_WATER_TEMP(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  (void)bytes;
  (void)sensor;

  const float B = 3988.0f;
  const float R0 = 10000.0f;
  const float T0 = 298.15f;
  const float R1 = 10000.0f;

  uint16_t v5_line = GET_5V_LINE(V5_LINE_PIN);
  float Vs = (v5_line > 0) ? (float)v5_line : 5000.0f;

  const float Vref = 3300.0f;

  float v = ((float)raw / 4095.0f) * Vref;

  if (v <= 0.0f)
    v = 0.001f;

  if (v >= Vs)
    v = Vs - 0.001f;

  float r = (v * R1) / (Vs - v);

  float temp_kelvin = 1.0f / ((1.0f / T0) + (logf(r / R0) / B));
  float temp_celsius = temp_kelvin - 273.15f;

  sensor->data = (uint16_t)temp_celsius;
  return (uint16_t)temp_celsius;
}

uint16_t TF_SUSP_TRAVEL(uint8_t bytes, uint32_t raw, Sensor* sensor)
{
  const uint16_t sensor_stroke = 1000;

  uint16_t raw_extended = 1100;
  uint16_t raw_compressed = 1950;

  get_calib_values(sensor, &raw_extended, &raw_compressed);

  uint16_t raw_min = raw_extended < raw_compressed ? raw_extended : raw_compressed;
  uint16_t raw_max = raw_extended < raw_compressed ? raw_compressed : raw_extended;

  raw = ValueControl(raw, raw_min, raw_max);

  int32_t raw_range = (int32_t)raw_compressed - (int32_t)raw_extended;

  if (raw_range == 0)
  {
    sensor->data = 0;
    return 0;
  }

  int32_t travel = ((int32_t)raw - (int32_t)raw_extended) * sensor_stroke / raw_range;

  if (travel < 0)
  {
    travel = 0;
  }

  if (travel > sensor_stroke)
  {
    travel = sensor_stroke;
  }

  sensor->data = (uint16_t)travel;
  return (uint16_t)travel;
}
