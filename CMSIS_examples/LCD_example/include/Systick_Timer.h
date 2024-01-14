#ifndef SYSTICK_TIMER_H
#define SYSTICK_TIMER_H

#include <stm32f411xe.h>
#include <stdint.h>

/**
 * @brief Funcoes para ativar o Systick Timer
 * para realizar as funcoes de atrasos (delays)
 * e para gerenciar outras coisas (caso queira)
 */

volatile uint32_t msTicks = 0;

/**
 * @brief Funcao de interrupcao do Systick TIMER
 * 
 * @note A funcao sera escrita no arquivo
 * "main.c" para poder colocar variaveis
 * ou funcoes para realizar os gerenciamentos
*/
void SysTick_Handler(void);

/**
 * @brief Funcao para iniciar a contagem so 
 * SysTick Timer
 * 
 * @note A divisao por 1000 serve para realizar o
 * estouro da interrupcao a cada 1 ms
*/
uint32_t Systick_Init()
{
    uint32_t returnCode;

    returnCode = SysTick_Config(SystemCoreClock / 1000);

    return returnCode;
}

#endif