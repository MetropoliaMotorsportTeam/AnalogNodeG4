#ifndef BOOT_H
#define BOOT_H
#include <stdint.h>

#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_fdcan.h"
#include "stm32g4xx_hal_rcc.h"

#define KiB(n) ((uint64_t)n << 10)
#define MiB(n) ((uint64_t)n << 20)
#define GiB(n) ((uint64_t)n << 30)

#define BOOT_ADDR FLASH_BASE
#define BOOT_SIZE KiB(32)
#define APP_SIZE KiB(96)
#define APP_ADDR (FLASH_BASE + BOOT_SIZE)

#define CAN_JUMP_BOOT (2)

#define MCU_IRQS 102u // no. of NVIC IRQ inputs
#define TIMEOUT 1000

typedef enum States
{
  ENTER_BOOTLOADER,
  ENTERING_BOOTLOADER,
  ENTERED_BOOTLOADER,
  BOOTLOADER_READY,
  FIRMWARE_SIZE,
  FIRMWARE_DATA,
  FIRMWARE_COMPLETE,
  RECEIVING_FIRMWARE,
  APP_START
} States;

void System_Jump(volatile uint32_t addr);

#endif /* BOOT_H */
