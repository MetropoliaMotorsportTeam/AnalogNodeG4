#ifndef FLASH_PEDAL_MAP_H
#define FLASH_PEDAL_MAP_H

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include "pedal_map.h"

#define PEDAL_FLASH_ADDR (0x0801F800U + FLASH_PAGE_SIZE)
#define PEDAL_FLASH_PAGE 128U

HAL_StatusTypeDef save_pedal_profile(uint16_t* pedal_profiles, uint8_t size);

#endif
