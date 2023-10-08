.syntax unified
.cpu cortex-m4

.equ RCC_AHB1ENR, 0x40023830
.equ GPIOx_MODER, 0x40020000
.equ GPIOx_ODR, 0x40020014

.global main

.section .text
main:
    // Enable clock for GPIOC
    ldr r0, =RCC_AHB1ENR
    ldr r1, [r0]
    orr r1, r1, #(1 << 2)    // Set bit 2 to enable GPIOC clock
    str r1, [r0]

    // Set GPIOC Pin 13 as output
    ldr r0, =GPIOx_MODER
    ldr r1, [r0]
    orr r1, r1, #(1 << 26)   // Set bits 27:26 to 01 for output mode
    str r1, [r0]

loop:
    // Toggle GPIOC Pin 13
    ldr r0, =GPIOx_ODR
    ldr r1, [r0]
    eor r1, r1, #(1 << 13)   // Toggle bit 13
    str r1, [r0]

    // Delay
    mov r2, #0
delay:
    adds r2, r2, #1
    cmp r2, #2          // Adjust the delay value as needed
    bne delay

    b loop

.end
