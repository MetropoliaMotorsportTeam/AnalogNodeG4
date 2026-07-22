#include "flash.h"
#include <string.h>

static uint8_t validate_flash_region(uint32_t addr, uint32_t size);

HAL_StatusTypeDef flash_erase_page(uint32_t mem_addr, uint8_t num_pages)
{

  if (num_pages == 0U)
  {
    return HAL_ERROR;
  }

  if (mem_addr < FLASH_BASE)
  {
    return HAL_ERROR;
  }

  FLASH_EraseInitTypeDef flash_erase = {0};
  uint32_t page_error;

  __disable_irq();
  HAL_FLASH_Unlock();

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  flash_erase.TypeErase = FLASH_TYPEERASE_PAGES;
  flash_erase.Banks = FLASH_BANK_1;
  flash_erase.Page = (mem_addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  flash_erase.NbPages = num_pages;

  HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&flash_erase, &page_error);

  HAL_FLASH_Lock();
  __enable_irq();
  return status;
}

HAL_StatusTypeDef flash_store(uint32_t addr, const void* data, uint32_t size)
{
  if ((addr % 8U) != 0U)
    return HAL_ERROR;

  if (data == NULL)
    return HAL_ERROR;

  if (size == 0)
    return HAL_ERROR;

  uint32_t aligned_size = ALIGN_TO(size, 8);
  const uint8_t* bytes = (const uint8_t*)data;

  // only allow writing of half the flash for safety
  if (aligned_size > FLASH_PAGE_SIZE * FLASH_PAGE_NB / 2)
    return HAL_ERROR;

  if (!validate_flash_region(addr, aligned_size))
    return HAL_ERROR;

  __disable_irq();
  HAL_FLASH_Unlock();

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  HAL_StatusTypeDef status = HAL_OK;
  for (uint32_t offset = 0; offset < aligned_size; offset += 8U)
  {
    uint64_t doubleword = 0xFFFFFFFFFFFFFFFFULL;
    uint8_t copy_size = 8U;

    if (offset + 8U > size)
    {
      copy_size = size - offset;
    }

    memcpy(&doubleword, &bytes[offset], copy_size);

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + offset, doubleword);
    if (status != HAL_OK)
    {
      break;
    }
  }

  HAL_FLASH_Lock();
  __enable_irq();

  return status;
}

static uint8_t validate_flash_region(uint32_t addr, uint32_t size)
{
  size = ALIGN_TO(size, 8);

  for (uint32_t offset = 0; offset < size; offset += 8U)
  {
    volatile uint64_t val = *(volatile uint64_t*)(addr + offset);
    if (val != FLASH_EMPTY_U64)
      return 0;
  }

  return 1;
}
