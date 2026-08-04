#ifndef GENERIC_FLASH_H
#define GENERIC_FLASH_H
#include "stm32g4xx_hal.h"

#define ALIGN_TO(base, align) (((base) + ((align) - (1))) & ~((align) - (1)))
#define FLASH_EMPTY_U64 0xFFFFFFFFFFFFFFFFULL

typedef enum FlashStatus
{
  FLASH_OK,
  FLASH_ERASE_ERROR,
  FLASH_WRITE_ERROR,
  FLASH_MISC_ERROR,
} FlashStatus;

FlashStatus flash_store(uint32_t addr, const void* data, uint32_t size);
FlashStatus flash_erase_page(uint32_t mem_addr, uint8_t num_pages);

#endif
