#include "flash_pedal_profiles.h"
#include "flash.h"
#include "main.h"
#include "stm32g4xx_it.h"
#include <stdint.h>
#include <string.h>
static uint16_t** get_saved_profiles(void);
static void restore_pedal_profile(uint8_t slot);

uint8_t validate_pedal_profile(uint16_t* pedal_profiles)
{
}

HAL_StatusTypeDef save_all_pedal_profiles(uint16_t** pedal_profiles)
{
  for (uint8_t slot = 0U; slot < PEDAL_PROFILE_SLOTS; slot++)
  {
    if (!validate_pedal_profile(pedal_profiles[slot]))
    {
      restore_pedal_profile(slot);
    }
  }

  if (memcmp(get_saved_profiles(), pedal_profiles, PEDAL_PROFILES_TOT_SIZE) == 0)
    return HAL_OK;

  if (flash_erase_page(PEDAL_FLASH_ADDR, 1) != HAL_OK)
    return HAL_ERROR;

  return HAL_OK;
}

static uint16_t** get_saved_profiles()
{
  volatile uint64_t* mem_ptr = (volatile uint64_t*)PEDAL_FLASH_ADDR;
  return (uint16_t**)mem_ptr;
}

static void restore_pedal_profile(uint8_t slot)
{
  if (slot >= PEDAL_PROFILE_SLOTS)
    return;
}
