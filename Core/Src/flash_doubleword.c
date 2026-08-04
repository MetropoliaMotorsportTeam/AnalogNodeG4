#include "flash_doubleword.h"
#include "config.h"
#include "flash.h"
#include "main.h"
#include "stm32g4xx_it.h"

/*
   Writes 8 bytes to the whole page before erasing the page
   Saves on costly flash erases
*/
FlashStatus save_dword(uint64_t dword, uint32_t base_addr)
{
  if (base_addr % FLASH_PAGE_SIZE != 0)
    // base_addr is not the beginning of a page
    return FLASH_MISC_ERROR;

  // if same data, don't write
  if (get_saved_dword(base_addr) == dword)
    return FLASH_OK;

  uint32_t addr = get_empty_dword_addr(base_addr);
  if (addr == INVALID_FLASH_ADDR)
  {
    FlashStatus status = flash_erase_page(addr, 1);
    if (status != FLASH_OK)
      return status;
  }

  return flash_store(addr, &dword, 1);
}

uint32_t get_empty_dword_addr(uint32_t base_addr)
{
  for (uint32_t i = 0; i < FLASH_PAGE_SLOTS; i++)
  {
    volatile uint64_t* mem_ptr = (volatile uint64_t*)base_addr;
    if (*mem_ptr == FLASH_EMPTY_U64)
    {
      return base_addr;
    }
    base_addr += sizeof(uint64_t);
  }
  return INVALID_FLASH_ADDR;
}
volatile uint64_t get_saved_dword(uint32_t base_addr)
{
  uint64_t last_valid_dword = FLASH_EMPTY_U64;

  for (uint32_t i = 0; i < FLASH_PAGE_SLOTS; i++)
  {
    volatile uint64_t* memptr = (volatile uint64_t*)base_addr;
    uint64_t data = *memptr;

    if (data == FLASH_EMPTY_U64)
    {
      return last_valid_dword;
    }

    last_valid_dword = data;

    base_addr += sizeof(uint64_t);
  }

  return last_valid_dword;
}
