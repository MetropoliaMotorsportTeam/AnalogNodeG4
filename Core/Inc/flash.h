#ifndef GENERIC_FLASH_H
#define GENERIC_FLASH_H
#include "main.h"
#include "stm32g4xx_it.h"

HAL_StatusTypeDef flash_store(uint32_t addr, uint64_t data);
HAL_StatusTypeDef flash_erase_page(uint32_t mem_addr, uint8_t num_pages);

#endif
