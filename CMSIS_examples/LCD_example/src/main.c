#include <stm32f411xe.h>
#include "Systick_Timer.h"

#include <LCD_stm32.h>

void SysTick_Handler(void)
{                               /* SysTick interrupt Handler. */
  msTicks++;                                        
}

int main(void)
{
    while(1)
    {
    }

    return 0;
}