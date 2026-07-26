#ifndef FLASH_PEDAL_MAP_H
#define FLASH_PEDAL_MAP_H

#include "pedal_map.h"
#include "stm32g4xx_hal.h"
#include <stdint.h>

#define PEDAL_FLASH_ADDR 0x0801F800U
#define PEDAL_FLASH_PAGE ((PEDAL_FLASH_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE)

PedalMapStatus
save_all_pedal_profiles(int16_t pedal_profiles[PEDAL_LUT_PROFILE_SLOTS][PEDAL_LUT_LENGTH]);
const int16_t* get_saved_profile(uint8_t slot);
PedalMapStatus restore_pedal_profile_from_flash(uint8_t slot, int16_t* pedal_profile);

#endif
