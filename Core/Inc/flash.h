#ifndef GENERIC_FLASH_H
#define GENERIC_FLASH_H
#include "stm32g4xx_hal.h"

#define ALIGN_TO(base, align) (((base) + ((align) - (1))) & ~((align) - (1)))
#define FLASH_EMPTY_U64 0xFFFFFFFFFFFFFFFFULL

HAL_StatusTypeDef flash_store(uint32_t addr, const void* data, uint32_t size);
HAL_StatusTypeDef flash_erase_page(uint32_t mem_addr, uint8_t num_pages);

#endif
