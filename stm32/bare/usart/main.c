// Standard library includes.
#include <stdint.h>
/*
#include <stdio.h>
#include <string.h>
*/

//uint32_t SystemCoreClock = 8000000;
//extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

//#define VVC_F1

// cribbed from blink sketch
#define RCC_BASE      	0x40021000
// reference page 51
#define GPIOA_BASE    	0x40010800
#define USART2_BASE	0x40004400

#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB1ENR_USART2EN	(1<<17)
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define RCC_APB2ENR_IOPAEN	(1<<2)
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)

#define GPIO_CRL_CNF2_Pos 10 // page 171
#define GPIO_CRL_CNF3_Pos 14
#define GPIO_CRL_MODE2_Pos 8
#define GPIO_CRL_MODE3_Pos 12
#define GPIO_CRL_CNF2 (0x3<<GPIO_CRL_CNF2_Pos)
#define GPIO_CRL_CNF3 (0x3<<GPIO_CRL_CNF3_Pos)
#define GPIO_CRL_MODE2 (0x3<<GPIO_CRL_MODE2_Pos)
#define GPIO_CRL_MODE3 (0x3<<GPIO_CRL_MODE3_Pos)
#define USART_BRR_DIV_Fraction_Pos 0 // page 820
#define USART_BRR_DIV_Mantissa_Pos 4
#define USART_CR1_RE (1<<2) // page 821
#define USART_CR1_TE (1<<3)
#define USART_CR1_UE (1<<13)
#define USART_SR_RXNE (1 << 5) // page 818
#define USART_SR_TXE (1 << 7)

// page 827
#define USART_SR 0x00
#define USART_DR 0x04
#define USART_BRR 0x08
#define USART_CR1 0x0C
#define USART_CR2 0x10
#define USART_CR3 0x14
#define USART_GTPR 0x18

#define USART2_SR *(volatile uint32_t *)(USART2_BASE+USART_SR)
#define USART2_DR *(volatile uint32_t *)(USART2_BASE+USART_DR)
#define USART2_BRR *(volatile uint32_t *)(USART2_BASE+USART_BRR)
#define USART2_CR1 *(volatile uint32_t *)(USART2_BASE+USART_CR1)

/**
 * Main program.
 */
int main( void ) {
	// Copy initialized data from .sidata (Flash) to .data (RAM)
	//memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
	// Clear the .bss section in RAM.
	//memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

	// Set the core system clock speed.
	// Default clock source is the 8MHz internal oscillator.
	uint32_t SystemCoreClock = 8000000;

	// Enable peripheral clocks and set up GPIO pins.
	// Enable peripheral clocks: GPIOA, USART2.
	RCC_APB1ENR  |=  ( RCC_APB1ENR_USART2EN );
	RCC_APB2ENR  |=  ( RCC_APB2ENR_IOPAEN );
	// Configure pins A2, A3 for USART2.
	GPIOA_CRL    &= ~( GPIO_CRL_MODE2 |
			GPIO_CRL_CNF2 |
			GPIO_CRL_MODE3 |
			GPIO_CRL_CNF3 );
	GPIOA_CRL    |= ( ( 0x1 << GPIO_CRL_MODE2_Pos ) |
			( 0x2 << GPIO_CRL_CNF2_Pos ) |
			( 0x0 << GPIO_CRL_MODE3_Pos ) |
			( 0x1 << GPIO_CRL_CNF3_Pos ) );

	// Set the baud rate to 9600.
	uint16_t uartdiv = SystemCoreClock / 9600;
	USART2_BRR = ( ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
			( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	// Enable the USART peripheral.
	USART2_CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );

	// Main loop: wait for a new byte, then echo it back.
	char rxb = '\0';
	while ( 1 ) {
		// Receive a byte of data.
		while( !( USART2_SR & USART_SR_RXNE ) ) {};
		rxb = USART2_DR;

		// Re-transmit the byte of data once the peripheral is ready.
		while( !( USART2_SR & USART_SR_TXE ) ) {};
		USART2_DR = rxb;
	}
}
