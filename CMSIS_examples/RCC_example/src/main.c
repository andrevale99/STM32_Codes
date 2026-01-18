#include <stm32f411xe.h>

int main(void)
{
    //Ativa e espera o HSE ficar pronto
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
        ;

    // Coloca o HSE como clock do sistema
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
        ;

    // Desativa o HSI para economizar energia
    RCC->CR &= ~RCC_CR_HSION;

    while (1)
    {
    }

    return 0;
}