#include <stm32f411xe.h>



//============================================================
//  VARIVEIS
//============================================================

//============================================================
//  PROTOTIPOS
//============================================================
void GPIO_Setup(void);
void TIMER3_Setup(void);

void TIMER3_IRQHandler(void);
//============================================================
//  MAIN
//============================================================
int main(void)
{
    GPIO_Setup();
    TIMER3_Setup();
    
    while(1)
    {
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

    GPIOC->ODR |= GPIO_ODR_OD13;
}

void TIMER3_Setup(void)
{
    //TIMER3 de 16 bits ativado
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //Carrega o valor maximo
    TIM3->ARR = 15635;

    TIM3->PSC = 1025;

    //Ativa o evento
    TIM3->EGR |= TIM_EGR_UG;

    TIM3->DIER |= TIM_DIER_UIE;

    //Ativa a contagem
    TIM3->CR1 |= TIM_CR1_URS | TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);
}

void TIMER3_IRQHandler(void)
{
    TIM3->CR1 |= TIM_CR1_CEN;
    GPIOC->ODR ^= GPIO_ODR_OD13;
}