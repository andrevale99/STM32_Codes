#include <stm32f411xe.h>

int main(void)
{
    // Ativa e espera o HSE ficar pronto
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
        ;

    // Coloca o HSE como clock do sistema
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
        ;

    // Desativa o HSI para economizar energia
    // RCC->CR &= ~RCC_CR_HSION;

    RCC->CFGR &= ~(RCC_CFGR_MCO1 | RCC_CFGR_MCO1PRE);
    // RCC->CFGR |= (0x0 << 21);     // HSI
    RCC->CFGR |= RCC_CFGR_MCO1_1; // HSE

    // Habilita clock do GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA8 em Alternate Function
    GPIOA->MODER |= (2 << 16);

    // AF0 para PA8 (MCO1)
    GPIOA->AFR[1] &= ~(0xF << 0);

    while (1)
    {
    }

    return 0;
}