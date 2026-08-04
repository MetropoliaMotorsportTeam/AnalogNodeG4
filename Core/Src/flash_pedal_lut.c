#include "flash_pedal_lut.h"
#include "flash.h"
#include "flash_doubleword.h"
#include "pedal_map.h"
#include <stdint.h>
#include <string.h>

uint8_t get_current_pedal_profile()
{
  uint8_t slot = get_saved_dword(PEDAL_CURR_PROFILE_FLASH_ADDR);
  if (slot >= PEDAL_LUT_PROFILE_SLOTS)
  {
    return 255;
  }
  return slot;
}

PedalMapStatus save_current_pedal_profile(uint8_t profile)
{
  if (profile >= PEDAL_LUT_PROFILE_SLOTS)
    return PEDAL_STATUS_INVALID_SLOT;

  if (!validate_pedal_profile(get_ram_pedal_profile(profile)))
  {
    return PEDAL_STATUS_INVALID_RAM_PROFILE;
  }

  if (save_dword(profile, PEDAL_CURR_PROFILE_FLASH_ADDR) != FLASH_OK)
  {
    return PEDAL_STATUS_FLASH_WRITE_FAILED;
  }
  return PEDAL_STATUS_OK;
}

PedalMapStatus
save_all_pedal_profiles(int16_t pedal_profiles[PEDAL_LUT_PROFILE_SLOTS][PEDAL_LUT_LENGTH])
{
  if (!pedal_profiles)
    return PEDAL_STATUS_NULL_PTR;

  for (uint8_t slot = 0; slot < PEDAL_LUT_PROFILE_SLOTS; slot++)
  {
    if (!validate_pedal_profile(pedal_profiles[slot]))
    {
      PedalMapStatus restore_status = restore_pedal_profile_from_flash(slot, pedal_profiles[slot]);

      if (restore_status != PEDAL_STATUS_OK)
      {
        return restore_status;
      }
    }
  }

  if (flash_erase_page(PEDAL_FLASH_ADDR, 1) != FLASH_OK)
  {
    return PEDAL_STATUS_FLASH_ERASE_FAILED;
  }

  if (flash_store(PEDAL_FLASH_ADDR, pedal_profiles, PEDAL_LUT_TOT_SIZE) != FLASH_OK)
  {
    return PEDAL_STATUS_FLASH_WRITE_FAILED;
  }

  return PEDAL_STATUS_OK;
}

PedalMapStatus restore_pedal_profile_from_flash(uint8_t slot, int16_t* pedal_profile)
{
  if (slot >= PEDAL_LUT_PROFILE_SLOTS)
    return PEDAL_STATUS_INVALID_SLOT;

  if (!pedal_profile)
    return PEDAL_STATUS_NULL_PTR;

  const int16_t* saved_profile = get_saved_pedal_profile(slot);

  if (!saved_profile)
    return PEDAL_STATUS_NULL_PTR;

  if (!validate_pedal_profile(saved_profile))
    return PEDAL_STATUS_INVALID_FLASH_PROFILE;

  memcpy(pedal_profile, saved_profile, PEDAL_LUT_SIZE_BYTES);
  return PEDAL_STATUS_OK;
}

const int16_t* get_saved_pedal_profile(uint8_t slot)
{
  if (slot >= PEDAL_LUT_PROFILE_SLOTS)
    return NULL;

  return (const int16_t*)(PEDAL_FLASH_ADDR + ((uint32_t)slot * PEDAL_LUT_SIZE_BYTES));
}

const int16_t* get_all_saved_pedal_profiles()
{
  return (const int16_t*)(PEDAL_FLASH_ADDR);
}
