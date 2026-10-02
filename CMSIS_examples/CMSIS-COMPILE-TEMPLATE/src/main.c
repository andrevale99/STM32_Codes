#include "stm32f4xx.h"

/* LED da Black Pill: PC13 (ativo em nivel baixo).
 * Para Nucleo-F411RE troque para GPIOA / pino 5 e RCC_AHB1ENR_GPIOAEN. */
#define LED_PORT        GPIOC
#define LED_PIN         13U
#define LED_CLK_EN      RCC_AHB1ENR_GPIOCEN

static volatile uint32_t ms_ticks = 0;

void SysTick_Handler(void)
{
    ms_ticks++;
}

static void delay_ms(uint32_t ms)
{
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms) {
        __NOP();
    }
}

static void led_init(void)
{
    RCC->AHB1ENR |= LED_CLK_EN;
    (void)RCC->AHB1ENR;                       /* atraso para o clock estabilizar */

    LED_PORT->MODER   &= ~(3U << (LED_PIN * 2U));
    LED_PORT->MODER   |=  (1U << (LED_PIN * 2U));   /* saida */
    LED_PORT->OTYPER  &= ~(1U << LED_PIN);          /* push-pull */
    LED_PORT->OSPEEDR &= ~(3U << (LED_PIN * 2U));   /* baixa velocidade */
    LED_PORT->PUPDR   &= ~(3U << (LED_PIN * 2U));   /* sem pull */
}

int main(void)
{
    /* Apos o reset o clock do sistema ja e o HSI (16 MHz).
     * SystemInit() (chamado no startup) mantem essa configuracao. */
    SystemCoreClockUpdate();

    /* SysTick a 1 kHz (interrupcao a cada 1 ms) */
    if (SysTick_Config(SystemCoreClock / 1000U) != 0U) {
        while (1) { }                         /* erro na configuracao */
    }

    led_init();

    while (1) {
        LED_PORT->ODR ^= (1U << LED_PIN);     /* alterna o LED */
        delay_ms(500);
    }
}