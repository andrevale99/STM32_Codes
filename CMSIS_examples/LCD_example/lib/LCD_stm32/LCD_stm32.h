#ifndef LCD_STM32_H
#define LCD_STM32_H

#include <stdint.h>

#define WRITE 1
#define CMD 0

#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define SECOND_LINE 0xC0

#define ENTRY_MODE_SET 0x04
#define INCREMENT (ENTRY_MODE_SET | 0x02) // I
#define DECREMENT (ENTRY_MODE_SET | 0x00) // D
#define SHIFT_DISPLAY (ENTRY_MODE_SET | 0x01) // S

#define DISPLAY_CONTROL 0x08
#define DISPLAY_OFF (DISPLAY_CONTROL | 0x00) // D
#define DISPLAY_ON (DISPLAY_CONTROL | 0x04) // D
#define CURSOR_ON (DISPLAY_CONTROL | 0x02) // C
#define BLINK_CURSOR (DISPLAY_CONTROL | 0x01) // B

#define CURSOR_SHIFT 0x10
#define DISPLAY_SHIFT (CURSOR_SHIFT | 0x08) // S
#define CURSOR_MOVE (CURSOR_SHIFT | 0x00) // C
#define SHIFT_RIGHT (CURSOR_SHIFT | 0x04) // R
#define SHIFT_LEFT (CURSOR_SHIFT | 0x00) // L

#define FUNCTION_SET 0x20
#define BITS_8 (FUNCTION_SET | 0x00) // DL
#define BITS_4 (FUNCTION_SET | 0x01) // DL
#define LINES_1 (FUNCTION_SET | 0x00) // N
#define LINES_2 (FUNCTION_SET | 0x08) // N
#define FONT_5x8 (FUNCTION_SET | 0x00) // F
#define FONT_5x10 (FUNCTION_SET | 0x04) // F

#define SET_DDRAM 0x80

/**
* @brief Inicializa o display LCD 16x2
* no protocolo de 4 bits
*/
void init_4bitsLCD();

/**
* @brief Funcao para envio de comandos e escrita 
* 
* @param cmd -> O comando do LCD ou caractere que deseja escrever
* @param write_or_cmd -> Verificacao se sera algum comando ou 
*                         escrita no LCD
*
* @note Para ESCRITA write_or_cmd==1 (WRITE), caso deseja enviar 
* algum COMANDO write_or_cmd==0 (CMD)
*/
void LCD_cmd(uint8_t cmd, uint8_t write_or_cmd);

/**
 * @brief Funcao para escrever uma string no LCD
 * 
 * @param str -> Ponteiro de um vetor de char
 * @param size -> tamanho do vetor
*/
void writeLCD(const char *str, const uint8_t size);

#endif