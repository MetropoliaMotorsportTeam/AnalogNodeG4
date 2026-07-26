#ifndef FLASH_PEDAL_MAP_H
#define FLASH_PEDAL_MAP_H

#include "pedal_map.h"
#include "stm32g4xx_hal.h"
#include <stdint.h>

#define PEDAL_FLASH_ADDR (0x0801F800U + FLASH_PAGE_SIZE)
#define PEDAL_FLASH_PAGE 128U

typedef enum
{
  PEDAL_PROFILE_STATUS_OK = 0,

  PEDAL_PROFILE_STATUS_NULL_PTR,
  PEDAL_PROFILE_STATUS_INVALID_SLOT,
  PEDAL_PROFILE_STATUS_INVALID_RAM_PROFILE,
  PEDAL_PROFILE_STATUS_INVALID_FLASH_PROFILE,

  PEDAL_PROFILE_STATUS_FLASH_ERASE_FAILED,
  PEDAL_PROFILE_STATUS_FLASH_WRITE_FAILED,

} PedalProfileStatus;

PedalProfileStatus
save_all_pedal_profiles(int16_t pedal_profiles[PEDAL_LUT_PROFILE_SLOTS][PEDAL_LUT_LENGTH]);
const int16_t* get_saved_profile(uint8_t slot);
PedalProfileStatus restore_pedal_profile_from_flash(uint8_t slot, int16_t* pedal_profile);

#endif
