#include <stm32f411xe.h>

//============================================================
//  VARIVEIS
//============================================================
volatile uint8_t flag = 0;
//============================================================
//  PROTOTIPOS
//============================================================
void GPIO_setup(void);

void EXTI15_10_IRQHandler(void);

void __attribute__((optimize("O0"))) delay (uint32_t time) 
{    
    static uint32_t i;
    for (i=0; i<time; i++) {}    
}
//============================================================
//  MAIN
//============================================================
int main(void)
{
    GPIO_setup();

    GPIOC->BSRR |= GPIO_BSRR_BS13;
    while(1)
    {
        if(flag)
        {
            for(uint8_t i=0; i<5; ++i)
            {
                GPIOC->BSRR |= GPIO_BSRR_BS13;
                delay(500000);
                GPIOC->BSRR |= GPIO_BSRR_BR13;
                delay(500000);
            }
            GPIOC->BSRR |= GPIO_BSRR_BS13;
            delay(100000);
            GPIOC->BSRR |= GPIO_BSRR_BR13;
            delay(100000);
            GPIOC->BSRR |= GPIO_BSRR_BS13;

            flag = 0;
        }
    }

    return 0;
}

//============================================================
//  FUNCOES
//============================================================
void GPIO_setup(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR|=  RCC_APB2LPENR_SYSCFGLPEN;

    GPIOC->MODER |= GPIO_MODER_MODER13_0;
    GPIOC->OTYPER   |= 0;
    GPIOC->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR13_1;

    GPIOB->MODER |= 0;
    GPIOB->OTYPER |= 0;
    GPIOB->OSPEEDR |= 0;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD13_0;

    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PB;
    EXTI->IMR |= EXTI_IMR_MR13;
    EXTI->FTSR |= EXTI_FTSR_TR13;
    EXTI->RTSR |= EXTI_RTSR_TR13;

    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    GPIOC->BSRR |= GPIO_BSRR_BR13;
    //__enable_irq();
}

void EXTI15_10_IRQHandler(void)
{
    flag = 1;

    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    EXTI->PR |= EXTI_PR_PR13;
}