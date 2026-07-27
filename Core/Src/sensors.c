#include "sensors.h"
#include "config.h"
#include "functions.h"
#include "main.h"
#include "stm32g4xx_hal_flash.h"
#include "transfer_functions.h"

Sensor* APPS1Pedal = {0};
Sensor* APPS2Pedal = {0};

static inline uint64_t pack_calib_values(Sensor* sensor)
{
  return ((uint64_t)sensor->high_adc << 16) | ((uint64_t)sensor->low_adc);
}

static inline uint8_t calib_flash_needs_update(void)
{
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    uint32_t flash_value = *(__IO uint32_t*)(ADC_CALIB_FLASH_ADDR + i * 8);
    uint32_t ram_value = (uint32_t)pack_calib_values(&sensors[i]);
    if (flash_value != ram_value)
      return 1;
  }
  return 0;
}

void init_sensors(void)
{
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    sensors[i].transfer_function = TF_3V3;
    sensors[i].CAN_ID = 0;
    sensors[i].CAN_interval = 20;
    sensors[i].averages = 0;
    sensors[i].pin = i;
    sensors[i].high_adc = 0xFFFF;
    sensors[i].low_adc = 0xFFFF;
    sensors[i].calib_code = 0;
  }
}

void ADC_Calib_Update()
{ // writes all 12 * 2 calibration values into the FLASH memory
  static FLASH_EraseInitTypeDef FlashErase;
  uint32_t PageError = 0;

  if (!calib_flash_needs_update())
    return;

  HAL_FLASH_Unlock();

  // Erase memory before writing
  //  Configure the flash erase parameters
  FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;
  FlashErase.Page = (ADC_CALIB_FLASH_ADDR / FLASH_PAGE_SIZE); // Convert address to page number
  FlashErase.NbPages = 1;

  __disable_irq();

  if (HAL_FLASHEx_Erase(&FlashErase, &PageError) != HAL_OK)
  {
    __enable_irq();
    HAL_FLASH_Lock();
    Error_Handler();
  }

  for (int i = 0; i < SENSOR_NUM; i++)
  {
    uint64_t data_to_write = pack_calib_values(&sensors[i]);
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ADC_CALIB_FLASH_ADDR + i * 8,
                          data_to_write) != HAL_OK)
    {
      Error_Handler();
    }
  }
  HAL_FLASH_Lock();
  __enable_irq();
}

void check_calib_status(Sensor* sensor)
{
  int8_t code = 0;
  if (sensor->low_adc != CALIB_DEFAULT)
    code += 1;
  if (sensor->high_adc != CALIB_DEFAULT)
    code += 2;

  sensor->calib_code = code;
}

void read_all_calib_values()
{

  for (int i = 0; i < SENSOR_NUM; i++)
  {
    uint32_t value = *(__IO uint32_t*)(ADC_CALIB_FLASH_ADDR + i * 8);

    uint16_t low = value;
    uint16_t high = value >> 16;

    sensors[i].low_adc = low;
    sensors[i].high_adc = high;

    int8_t code = 0;
    if (sensors[i].low_adc != CALIB_DEFAULT)
      code += 1;
    if (sensors[i].high_adc != CALIB_DEFAULT)
      code += 2;

    sensors[i].calib_code = code;
  }
}
