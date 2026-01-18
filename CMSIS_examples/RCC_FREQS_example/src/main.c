#include <stm32f411xe.h>

//============================================================
//  VARIVEIS
//============================================================
volatile uint8_t cnt = 0;
//============================================================
//  PROTOTIPOS
//============================================================
void CLOCK_setup(void);
void GPIO_Setup(void);
void TIMER3_Setup(void);

void TIM3_IRQHandler(void);
//============================================================
//  MAIN
//============================================================
int main(void)
{
    CLOCK_setup();
    GPIO_Setup();
    TIMER3_Setup();

    while (1)
    {
    }

    return 0;
}
//============================================================
//  FUNCOES
//============================================================

void CLOCK_setup(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
        ;
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
        ;
    RCC->CR &= ~RCC_CR_HSION;

    // Configura o APB1 com divisao por 8
    // RCC->CFGR |= RCC_CFGR_PPRE1_DIV8;
}

void GPIO_Setup(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER |= GPIO_MODER_MODE13_0;
    GPIOC->OTYPER = 0;
    GPIOC->OSPEEDR = 0;
    GPIOC->PUPDR = 0;

    GPIOC->BSRR |= GPIO_BSRR_BR13;
}

void TIMER3_Setup(void)
{
    // TIMER3 de 16 bits ativado
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Carrega o valor maximo da contagem
    // TIM3->ARR = 24414; // APB1 SEM DIVISAO
    TIM3->ARR = 6103; // APB1 COM DIVISAO POR 8

    // Ativa o prescale do contador
    TIM3->PSC = 1025;

    // Valor de comaracao (Duty Cycle)
    TIM3->CCR1 = 1;

    // Ativa o TIMER como PWM 1 e ativa o preload
    TIM3->CCMR1 = ((0x6UL << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE);

    // Ativa a interrupcao por comparacao
    TIM3->CCER |= TIM_CCER_CC1E;

    // Ativa o evento somente pelo Contador
    TIM3->EGR |= TIM_EGR_UG;

    // Ativa a interrupcao
    TIM3->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE);

    // Ativa a interrupcao somente pelo overflow/underflow ou DMA
    // e ativa o contador
    TIM3->CR1 |= (TIM_CR1_ARPE | TIM_CR1_URS);

    TIM3->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);
}

void TIM3_IRQHandler(void)
{

    if (TIM3->SR & TIM_SR_CC1IF)
    {
        TIM3->SR &= ~TIM_SR_CC1IF;
        GPIOC->ODR ^= GPIO_ODR_OD13;

        // Sempre desativar
        TIM3->SR &= (~TIM_SR_UIF);
    }
}