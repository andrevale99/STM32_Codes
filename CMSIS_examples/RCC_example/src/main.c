#include <stm32f411xe.h>

int main(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
        ;

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    __DSB();

    GPIOC->MODER |= GPIO_MODER_MODE13_0;
    GPIOC->BSRR |= GPIO_BSRR_BS13;
    GPIOC->OTYPER = 0;
    GPIOC->OSPEEDR = 0;
    GPIOC->PUPDR = 0;

    // No black PILL o led PC13 ativa no 0
    if ((RCC->CR & RCC_CR_HSERDY))
        GPIOC->ODR &= ~GPIO_ODR_OD13;
    else
        GPIOC->ODR |= GPIO_ODR_OD13;

    while (1)
    {
    }

    return 0;
}