#include "flash_conf.h"
#include "config.h"
#include "flash.h"
#include "main.h"
#include "stm32g4xx_it.h"

static uint32_t get_empty_conf_addr();

HAL_StatusTypeDef save_config(uint8_t config)
{
  if (!valid_config(config))
    config = DEFAULT_CONF;

  if (get_curr_conf() == config)
    return HAL_OK;

  uint32_t addr = get_empty_conf_addr();
  if (addr == 0)
  {
    // flash full
    if (flash_erase_page(CONFIG_FLASH_ADDR, 1) != HAL_OK)
      return HAL_ERROR;

    addr = CONFIG_FLASH_ADDR;
  }
  return flash_store(addr, make_config_record(config));
}

static uint32_t get_empty_conf_addr()
{
  uint32_t addr = CONFIG_FLASH_ADDR;

  for (uint32_t i = 0; i < CONFIG_NUM_SLOTS; i++)
  {
    volatile uint64_t* mem_ptr = (volatile uint64_t*)addr;
    if (*mem_ptr == FLASH_EMPTY_U64)
    {
      return addr;
    }
    addr += CONFIG_SLOT_SIZE;
  }
  return 0;
}
volatile uint8_t get_curr_conf(void)
{
  uint32_t addr = CONFIG_FLASH_ADDR;
  uint8_t last_valid_conf = DEFAULT_CONF;

  for (uint32_t i = 0; i < CONFIG_NUM_SLOTS; i++)
  {
    volatile uint64_t* mem_ptr = (volatile uint64_t*)addr;
    uint64_t record = *mem_ptr;

    if (record == FLASH_EMPTY_U64)
    {
      return last_valid_conf;
    }

    uint8_t conf = config_from_record(record);

    if (valid_config(conf))
    {
      last_valid_conf = conf;
    }

    addr += CONFIG_SLOT_SIZE;
  }

  return last_valid_conf;
}
