#include "sensors.h"
#include "config.h"
#include "stm32g4xx_hal_flash.h"
#include "transfer_functions.h"

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
  }
}

void ADC_Calib_Update()
{ // writes all 12 * 2 calibration values into the FLASH memory
  static FLASH_EraseInitTypeDef FlashErase;
  uint32_t PageError = 0;

  // Unlock the Flash memory
  HAL_FLASH_Unlock();

  // Erase memory before writing
  //  Configure the flash erase parameters
  FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;
  FlashErase.Page = (FLASH_ADDRESS / FLASH_PAGE_SIZE); // Convert address to page number
  FlashErase.NbPages = 1;

  __disable_irq();

  // Perform the erase operation
  if (HAL_FLASHEx_Erase(&FlashErase, &PageError) != HAL_OK)
  {
    // Handle the error
    Error_Handler();
  }

  for (int i = 0; i < SENSOR_NUM; i++)
  {
    // Prepare data to write
    uint64_t data_to_write = (sensors[i].high_adc << 16) | sensors[i].low_adc;
    // Program the flash memory
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_ADDRESS + i * 8, data_to_write) !=
        HAL_OK)
    { // TODO Figure out how to write just 32 bits without a need for filler 0s
      Error_Handler();
    }
  }
  // Lock the Flash memory after operation
  HAL_FLASH_Lock();
  // Enable interrupts after the operation
  __enable_irq();
}

void check_calib_status(Sensor* sensor)
{

  uint16_t default_value = 0xFFFF;
  int8_t code = 3;
  if (sensor->low_adc == default_value)
    code += 1;
  if (sensor->high_adc == default_value)
    code += 2;

  sensor->calib_code = code;
}

void read_all_calib_values()
{

  for (int i = 0; i < SENSOR_NUM; i++)
  {

    uint32_t value = *(__IO uint32_t*)(FLASH_ADDRESS + i * 8);

    uint16_t low = value;
    uint16_t high = value >> 16;

    sensors[i].low_adc = low;
    sensors[i].high_adc = high;

    uint16_t default_value = 0xFFFF;
    int8_t code = 0;
    if (sensors[i].low_adc != default_value)
      code += 1;
    if (sensors[i].high_adc != default_value)
      code += 2;

    sensors[i].calib_code = code;

    check_calib_status(&sensors[i]);
  }
}
