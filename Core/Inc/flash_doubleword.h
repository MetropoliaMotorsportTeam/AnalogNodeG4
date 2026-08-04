#ifndef FLASH_CONFIG_H
#define FLASH_CONFIG_H
/*
  - Flash write file to store which dworduration should be used
  - Uses the last Page of FLash memory to store
  - NOTE: Do not overwrite the last page

*/
#include "stm32g4xx_hal.h"
#include <stdint.h>

#define FLASH_PAGE_SLOTS (FLASH_PAGE_SIZE / sizeof(uint64_t))

static inline uint64_t flash_read_u64(uint32_t addr)
{
  return *(volatile uint64_t*)addr;
}

// converts 8bit into 64bit with empty flash
static inline uint64_t fill_64(uint8_t dword)
{
  return 0xFFFFFFFFFFFFFF00ULL | (uint64_t)dword;
}

HAL_StatusTypeDef save_dword(uint64_t dword, uint32_t base_addr);
volatile uint64_t get_saved_dword(uint32_t base_addr);
uint32_t get_empty_dword_addr(uint32_t base_addr);

#endif
