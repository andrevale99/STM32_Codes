#include <stm32f411xe.h>

#define FREQ 5

//============================================================
//  VARIVEIS
//============================================================
volatile uint8_t cnt = 0;
//============================================================
//  PROTOTIPOS
//============================================================
void GPIO_Setup(void);
void TIMER3_Setup(void);

void TIM3_IRQHandler(void);
//============================================================
//  MAIN
//============================================================
int main(void)
{
    GPIO_Setup();
    TIMER3_Setup();
    
    while(1)
    {
        /*if(TIM3->CNT > 5000)
            GPIOC->BSRR |= GPIO_BSRR_BR13;
        else
            GPIOC->BSRR |= GPIO_BSRR_BS13;*/
    }

    return 0;
}
//============================================================
//  FUNCOES
//============================================================
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
    //TIMER3 de 16 bits ativado
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //Carrega o valor maximo da contagem
    TIM3->ARR = (uint16_t)15635/FREQ;

    //Ativa o prescale do contador
    TIM3->PSC = 1025;

    //Ativa o evento somente pelo Contador
    TIM3->EGR |= TIM_EGR_UG;

    //Ativa a interrupcao
    TIM3->DIER |= TIM_DIER_UIE;

    //Ativa a interrupcao somente pelo overflow/underflow ou DMA
    //e ativa o contador
    TIM3->CR1 |= TIM_CR1_URS | TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);
}

void TIM3_IRQHandler(void)
{
    //Sempre desativar antes de fazer qualquer coisa
	TIM3->SR &= ~TIM_SR_UIF;

    GPIOC->ODR ^= GPIO_ODR_OD13;
}
