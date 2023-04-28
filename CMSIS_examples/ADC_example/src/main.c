#include <stm32f411xe.h>

//============================================================
//  VARIVEIS
//============================================================
volatile uint16_t adc_irq = 0;
//============================================================
//  PROTOTIPOS
//============================================================
void GPIO_Setup(void);
void ADC_Setup(void);

void ADC_IRQHandler(void);
//============================================================
//  MAIN
//============================================================
int main(void)
{
    GPIO_Setup();
    ADC_Setup();
    
    uint16_t adc_value = 0;
    while(1)
    {
        //Logica para caso n usa a interrupcao
        /*while(!(ADC1->SR & ADC_SR_EOC))
            ;

        adc_value = ADC1->DR;*/

        if((adc_value > 2000) | (adc_irq > 2000))
            GPIOC->BSRR |= GPIO_BSRR_BR13;
        else
            GPIOC->BSRR |= GPIO_BSRR_BS13;
        

    }

    return 0;
}
//============================================================
//  FUNCOES
//============================================================
void GPIO_Setup(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;

    GPIOC->MODER |= GPIO_MODER_MODE13_0;
    GPIOC->OTYPER = 0;
    GPIOC->OSPEEDR = 0;
    GPIOC->PUPDR = 0;

    GPIOC->BSRR |= GPIO_BSRR_BS13;

    //Modo analogico no pino A0
    GPIOA->MODER |= (3 << GPIO_MODER_MODE0_Pos);
}

void ADC_Setup(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    //Deve iniciar o ADC antes de iniciar 
    //as conversões
    ADC1->CR2 |= ADC_CR2_ADON;

    //Ativa o modo continuo e Inicia a conversao
    //nos pinos regulares
    ADC1->CR2 |= ADC_CR2_SWSTART | ADC_CR2_CONT;

    //Ativa a interrupcao
    ADC1->CR1 |= ADC_CR1_EOCIE;
    NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void)
{
    adc_irq = ADC1->DR;
}