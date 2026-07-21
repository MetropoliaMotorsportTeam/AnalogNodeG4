#include "flash.h"

HAL_StatusTypeDef flash_erase_page(uint32_t mem_addr, uint8_t num_pages)
{
  if ((mem_addr % 8U) != 0U)
  {
    return HAL_ERROR;
  }

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

// TODO: update this to handle larger amount of data
HAL_StatusTypeDef flash_store(uint32_t addr, uint64_t data)
{
  if ((addr % 8U) != 0U)
  {
    return HAL_ERROR;
  }

  __disable_irq();
  HAL_FLASH_Unlock();

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data);

  HAL_FLASH_Lock();
  __enable_irq();

  return status;
}
