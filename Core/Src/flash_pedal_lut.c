#include "flash_pedal_lut.h"
#include "flash.h"
#include "main.h"
#include "stm32g4xx_it.h"
#include <stdint.h>
#include <string.h>

PedalProfileStatus
save_all_pedal_profiles(int16_t pedal_profiles[PEDAL_PROFILE_SLOTS][PEDAL_LUT_SIZE])
{
  if (!pedal_profiles)
    return PEDAL_PROFILE_STATUS_NULL_PTR;

  for (uint8_t slot = 0; slot < PEDAL_PROFILE_SLOTS; slot++)
  {
    if (!validate_pedal_profile(pedal_profiles[slot]))
    {
      PedalProfileStatus restore_status =
          restore_pedal_profile_from_flash(slot, pedal_profiles[slot]);

      if (restore_status != PEDAL_PROFILE_STATUS_OK)
      {
        return restore_status;
      }
    }
  }

  if (flash_erase_page(PEDAL_FLASH_ADDR, 1) != HAL_OK)
  {
    return PEDAL_PROFILE_STATUS_FLASH_ERASE_FAILED;
  }

  if (flash_store(PEDAL_FLASH_ADDR, pedal_profiles, PEDAL_PROFILES_TOT_SIZE) != HAL_OK)
  {
    return PEDAL_PROFILE_STATUS_FLASH_WRITE_FAILED;
  }

  return PEDAL_PROFILE_STATUS_OK;
}

PedalProfileStatus restore_pedal_profile_from_flash(uint8_t slot, int16_t* pedal_profile)
{
  if (slot >= PEDAL_PROFILE_SLOTS)
    return PEDAL_PROFILE_STATUS_INVALID_SLOT;

  if (!pedal_profile)
    return PEDAL_PROFILE_STATUS_NULL_PTR;

  const int16_t* saved_profile = get_saved_profile(slot);

  if (!saved_profile)
    return PEDAL_PROFILE_STATUS_NULL_PTR;

  if (!validate_pedal_profile(saved_profile))
    return PEDAL_PROFILE_STATUS_INVALID_FLASH_PROFILE;

  memcpy(pedal_profile, saved_profile, PEDAL_PROFILE_SIZE);
  return PEDAL_PROFILE_STATUS_OK;
}

const int16_t* get_saved_profile(uint8_t slot)
{
  if (slot >= PEDAL_PROFILE_SLOTS)
    return NULL;

  return (const int16_t*)(PEDAL_FLASH_ADDR + ((uint32_t)slot * PEDAL_PROFILE_SIZE));
}
