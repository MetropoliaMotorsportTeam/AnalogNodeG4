#include "config.h"
#include "flash_conf.h"
#include "sensors.h"
#include "transfer_functions.h"
#include "virtual_sensors.h"
#include <string.h>

static void Config_1(void);
static void Config_2(void);
static void Config_3(void);
static void Config_4(void);

void Config_Setup(void)
{
  init_sensors();
  Load_Config();
  read_all_calib_values();
}

void Apply_Config(uint8_t config)
{
  switch (config)
  {
  case 1:
    Config_1();
    break;
  case 2:
    Config_2();
    break;
  case 3:
    Config_3();
    break;
  case 4:
    Config_4();
    break;
  default:
    break;
  }
}

void Load_Config()
{
  /* uint8_t config = get_curr_conf();
  uint8_t conf = ((config < 1 || config > NUM_CONF) ? DEFAULT_CONF : config);
  Apply_Config(conf); */
  Apply_Config(2);
}

void Process_Config(uint8_t config)
{
  uint8_t conf = ((config < 1 || config > NUM_CONF) ? DEFAULT_CONF : config);
  if (save_config(conf) != HAL_OK)
    Error_Handler();

  Apply_Config(conf);
}

static void Config_1(void)
{
  Sensor BTN1 = {TF_BTN, 2, 100, 0, V5_in0};
  Sensor BTN2 = {TF_BTN, 13, 100, 0, V5_in5};
  Sensor BTN3 = {TF_BTN, 14, 100, 0, V5_in4};
  Sensor APPS2 = {TF_APPS2, 2, 100, 0, V5_in1};
  Sensor APPS1 = {TF_APPS1, 1, 100, 0, V24_in1};
  Sensor ANGLE = {TF_24V, 6, 100, 0, V24_in0};
  Sensor BPPS = {TF_BPPS, 3, 100, 0, V5_in2};
  Sensor F_ROLL = {TF_5V, 10, 100, 0, V5_in6};
  Sensor F_HEAVE = {TF_5V, 8, 100, 0, V5_in3};

  sensors[APPS1.pin] = APPS1;
  sensors[APPS2.pin] = APPS2;
  sensors[BPPS.pin] = BPPS;
  sensors[BTN1.pin] = BTN1;
  sensors[BTN2.pin] = BTN2;
  sensors[BTN3.pin] = BTN3;
  sensors[ANGLE.pin] = ANGLE;
  sensors[F_ROLL.pin] = F_ROLL;
  sensors[F_HEAVE.pin] = F_HEAVE;

  CAN_interval = 100;
}

static void Config_2(void)
{
  // switch to V24_in1 if V5 doesn't work
  Sensor APPS2 = {TF_APPS_TEST, 2, 20, 0, V5_in0};
  sensors[APPS2.pin] = APPS2;
  APPS_pedal = &sensors[APPS2.pin];
  CAN_interval = 100;
}

static void Config_3(void)
{
  Sensor APPS1 = {TF_APPS1, 1, 100, 0, V5_in0};
  Sensor APPS2 = {TF_APPS2, 2, 100, 0, V5_in1};
  sensors[APPS1.pin] = APPS1;
  sensors[APPS2.pin] = APPS2;

  pedalreq.input_count = 2;
  pedalreq.inputs[0] = &sensors[APPS1.pin];
  pedalreq.inputs[1] = &sensors[APPS2.pin];

  init_virtual_sensor(&pedalreq, "pedal request", update_pedalreq, NULL, 17);
  add_input_sensor(&pedalreq, &sensors[APPS1.pin]);
  add_input_sensor(&pedalreq, &sensors[APPS2.pin]);

  CAN_interval = 100;
}

static void Config_4(void)
{
  Sensor W_TEMP = {TF_WATER_TEMP, 2, 100, 0, V5_in0};
  Sensor V5_LINE = {TF_5V, 10, 100, 0, V5_LINE_PIN};

  sensors[W_TEMP.pin] = W_TEMP;
  sensors[V5_LINE.pin] = V5_LINE;

  CAN_interval = 100;
}
