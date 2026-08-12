#include "bootloader.h"
#include "fdcan.h"
#include "functions.h"
#include "main.h"

extern CAN_Message RxMessage;
extern CAN_Message TxMessage;

extern FDCAN_TxHeaderTypeDef TxHeader;
extern FDCAN_RxHeaderTypeDef RxHeader;

void System_Jump(volatile uint32_t addr)
{
  /*
1. disable interrupts
2. reset system
3. reset stack pointer (MSP) -> eg: 0x08000000
4. jump to reset handler -> MSP + 4U (2nd element in vec_table)
*/

  // disable interrupts
  __disable_irq();
  HAL_FDCAN_Stop(&hfdcan1);

  uint32_t msp = *(volatile uint32_t*)addr;
  uint32_t reset_handler = *(volatile uint32_t*)(addr + 4U);
  void (*reset_handler_fn)(void) = (void (*)(void))reset_handler;

  HAL_RCC_DeInit();
  HAL_DeInit();
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

  /* Clear Interrupt Enable Register & Interrupt Pending Register */
  for (uint8_t i = 0; i < (MCU_IRQS + 31u) / 32; i++)
  {
    NVIC->ICER[i] = 0xFFFFFFFF;
    NVIC->ICPR[i] = 0xFFFFFFFF;
  }

  // set Vector Table Offset Register
  SCB->VTOR = addr;
  __DSB();
  __ISB();

  __set_MSP(msp);
  reset_handler_fn();
}
