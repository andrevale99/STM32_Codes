/**
 * @author Andre Menezes de Freitas Vale
 * 
 * @brief Algoritmo que utiliza das interrupcoes para enviar
 * dados pela serial USART0 do ATmega328p
 * 
 * @note A Funcao USART_transmit nao utiliza da interrupcao
 * para enviar o dado, esta aqui pq copiei e colei caso alguem necessite
 * utiliza-la e esta com preguica de pegar do datasheet :)
 * 
 * @note O codigo utiliza de um botao no PB0 com PULL-UP o qual serve 
 * para ativar o comeco da transmissao, ao aterrar o PB0. Logica do codigo 
 * nos comentarios no codigo
 * 
*/

#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdbool.h>

#define SetBit(port, pin) (port |= (1<<pin))
#define ClrBit(port, pin) (port &= ~(1<<pin))
#define ToggleBit(port, pin) (port ^= (1<<pin))
#define TestBit(port, pin) (port & (1<<pin))

#define BAUD 115200
#define MYUBRR F_CPU/16/BAUD-1

//===================================================
//  VARIAVEIS
//===================================================
volatile uint8_t rx_data = 0;
//===================================================
//  PROTOTIPOS
//===================================================
void setup();

void USART_Init(unsigned int ubrr);

ISR(USART_RX_vect);
//===================================================
//  MAIN
//===================================================
int main()
{
    setup();
    sei();

    for(;;)
    {
        if(rx_data % 2 == 0 && rx_data != 0)
            ToggleBit(PORTB, PB5);
    }

    return 0;
}

//===================================================
//  FUNCOES
//===================================================
/**
 * @brief Funcao de setup
*/
void setup()
{
    SetBit(DDRB, DDB5);
    
    ClrBit(PORTB, PB5);

    USART_Init(MYUBRR);
}

/**
 * @brief Inicializacao do USART
 * @param ubrr Valor calculado do baud rate
 * 
 * @note A definicao MYUBRR ja faz o calculo (datasheet)
 * e inicializa somento o TX, para o RX (1<<RXEN0) no
 * registrador UCSR0B
*/
void USART_Init(unsigned int ubrr)
{
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;

    /*Enable transmitter 
    Desativa a interrupcao Data Empty
    Desativa a interrupcao TX complete*/
    SetBit(UCSR0B, RXEN0);
    SetBit(UCSR0B, RXCIE0);

    ClrBit(UCSR0B, TXEN0);
    ClrBit(UCSR0B, TXCIE0);
    ClrBit(UCSR0B, UDRIE0);

    /* Set frame format: 8data, 2stop bit */
    SetBit(UCSR0C, USBS0);
    SetBit(UCSR0C, UCSZ00);
}

/**
 * @brief USART, Rx complete
 * 
 * @note Entra nesta interrupcao quando a 
 * Recepcao for completa
*/
ISR(USART_RX_vect)
{
    rx_data = UDR0;
}