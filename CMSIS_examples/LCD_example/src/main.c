#include <stm32f411xe.h>
#include "SystickTimer_custom.h"

#include <LCD_stm32.h>

void SysTick_Handler(void)
{ /* SysTick interrupt Handler. */
  msTicks++;
}

void delay_ms(uint32_t ms)
{
  uint32_t start = msTicks;
  while ((msTicks - start) != ms)
    ;
}

int main(void)
{
  Systick_Init(1000);
  
  while (1)
  {
  }

  return 0;
}