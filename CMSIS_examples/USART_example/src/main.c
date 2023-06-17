#include <stm32f411xe.h>

//============================================================
//  VARIVEIS
//============================================================
volatile uint32_t data = 0;
//============================================================
//  PROTOTIPOS
//============================================================
void USART_setup();
void USART2_IRQHandler(void);
//============================================================
//  MAIN
//============================================================
int main(void)
{

    USART_setup();

    while(1)
    {
    }

    return 0;
}
//============================================================
//  FUNCOES
//============================================================
void USART_setup()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    //PINO A3 como RX
    GPIOA->MODER |= GPIO_MODER_MODER3_1;
    GPIOA->AFR[0] |= (7<<GPIO_AFRL_AFSEL3_Pos);
    
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    //Configura o Baud rate do protocolo
    uint32_t baud_rate = 115200;
    uint16_t uartdiv = SystemCoreClock / baud_rate;
    USART2->BRR = ( ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
    				( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

    //Ativa a interrupcao e somente o RX do MCU
    USART2->CR1 |= USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;

    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 0);
}

void USART2_IRQHandler(void)
{
    //Bit de interrupcao limpo quando o registrador
    //de dado é lido
    if((USART2->SR & USART_SR_RXNE))
        data = USART2->DR;
}