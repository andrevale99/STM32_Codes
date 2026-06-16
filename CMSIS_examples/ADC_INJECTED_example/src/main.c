#include <stm32f411xe.h>

volatile uint16_t ia = 0;
volatile uint16_t ib = 0;

void tim1_setup(void)
{
    /* Habilita clock do TIM1 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* Desabilita timer durante configuração */
    TIM1->CR1 = 0;

    /* Prescaler = 0 */
    TIM1->PSC = 0;

    /* ARR para 10 kHz */
    TIM1->ARR = 799;

    /* Duty inicial = 50% */
    TIM1->CCR1 = 400;

    /* PWM Mode 1 no canal 1 */
    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M);
    TIM1->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);

    /* Preload CCR1 */
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;

    /* Habilita saída CH1 */
    TIM1->CCER |= TIM_CCER_CC1E;

    /* Center-aligned mode 1 */
    TIM1->CR1 &= ~(TIM_CR1_CMS);
    TIM1->CR1 |= TIM_CR1_CMS_0;

    /* Auto-reload preload */
    TIM1->CR1 |= TIM_CR1_ARPE;

    /* Necessário para TIM1/TIM8 */
    TIM1->BDTR |= TIM_BDTR_MOE;

    /* Atualiza registradores */
    TIM1->EGR |= TIM_EGR_UG;

    /* Inicia timer */
    TIM1->CR1 |= TIM_CR1_CEN;
}

void adc_injected_setup(void)
{
    /*--------------------------------------------------
     * Clocks
     *-------------------------------------------------*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /*--------------------------------------------------
     * PA6 e PA7 em modo analógico
     *-------------------------------------------------*/
    GPIOA->MODER |= (3 << GPIO_MODER_MODER6_Pos);
    GPIOA->MODER |= (3 << GPIO_MODER_MODER7_Pos);

    GPIOA->PUPDR &= ~(3 << GPIO_PUPDR_PUPD6_Pos);
    GPIOA->PUPDR &= ~(3 << GPIO_PUPDR_PUPD7_Pos);

    /*--------------------------------------------------
     * ADC desligado durante configuração
     *-------------------------------------------------*/
    ADC1->CR2 &= ~ADC_CR2_ADON;

    /*--------------------------------------------------
     * Prescaler ADC
     * APB2 = 16 MHz
     * ADC Clock = 8 MHz
     *-------------------------------------------------*/
    ADC->CCR &= ~ADC_CCR_ADCPRE;
    ADC->CCR |= ADC_CCR_ADCPRE_0;

    /*--------------------------------------------------
     * Sample Time
     * 84 ciclos para reduzir ruído
     * Canal 6
     * Canal 7
     *-------------------------------------------------*/
    ADC1->SMPR2 &= ~(
        (7 << (3 * 6)) |
        (7 << (3 * 7)));

    ADC1->SMPR2 |=
        (4 << (3 * 6)) |
        (4 << (3 * 7));

    /*--------------------------------------------------
     * Trigger externo injected
     *
     * JEXTSEL = 0001 = TIM1_TRGO
     * JEXTEN  = 01   = Rising Edge
     *-------------------------------------------------*/
    ADC1->CR2 &= ~(
        ADC_CR2_JEXTSEL |
        ADC_CR2_JEXTEN);

    ADC1->CR2 |=
        (0x1 << ADC_CR2_JEXTSEL_Pos) |
        (0x1 << ADC_CR2_JEXTEN_Pos);

    /*--------------------------------------------------
     * Sequência Injected
     *
     * JL = 1 => 2 conversões
     *-------------------------------------------------*/
    ADC1->JSQR = 0;

    ADC1->JSQR |= (1 << 20);

    /*
     * Para JL=1:
     *
     * JSQ2 = primeira conversão
     * JSQ1 = segunda conversão
     */

    ADC1->JSQR |= (6 << 5); // PA6 -> primeira
    ADC1->JSQR |= (7 << 0); // PA7 -> segunda

    ADC1->CR1 |= ADC_CR1_JEOCIE;

    NVIC_EnableIRQ(ADC_IRQn);

    /*--------------------------------------------------
     * Habilita ADC
     *-------------------------------------------------*/
    ADC1->CR2 |= ADC_CR2_ADON;
}

void ADC_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_JEOC)
    {
        ADC1->SR &= ~ADC_SR_JEOC;
        ia = ADC1->JDR1;
        ib = ADC1->JDR2;
    }
}

int main(void)
{
    tim1_setup();
    adc_injected_setup();

    while (1)
    {
        // while (!(ADC1->SR & ADC_SR_JEOC))
        //     ;
        // uint16_t correnteA = ADC1->JDR1;
        // uint16_t correnteB = ADC1->JDR2;
        // ADC1->SR &= ~ADC_SR_JEOC;
    }

    return 0;
}