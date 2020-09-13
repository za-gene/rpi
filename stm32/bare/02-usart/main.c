#include <string.h>
#include <stddef.h> // for size_t
#include <stdio.h>
#include "../blue.h"

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

// cribbed from blink sketch
#define RCC_BASE      	0x40021000
// reference page 51
//#define GPIOA_BASE    	0x40010800

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


#define USART2	((USART_t*) 0x40004400)


char greeting[] = "Hello from bare metal usart 8";

void* memcpy_usr(void* dst, const void* src, size_t n) {
	// Copies n bytes from src to dst

	// Since we cannot dereference a void* ptr,
	// we first typecast it to a char* ptr
	// and then do the copying byte by byte,
	// since a char* ptr references a single byte
	char* char_dst = (char*) dst;
	char* char_src = (char*) src;

	for (int i=0; i<n; i++) {
		*char_dst++ = *char_src++;
	}

	return dst;
}

void *memset_usr(void *b, int c, int len)
{
	int           i;
	unsigned char *p = b;
	i = 0;
	while(len > 0)
	{
		*p = c;
		p++;
		len--;
	}
	return(b);
}

void init_mem()
{
	// not called explicitly, but done in crt.s prior to calling main()

	// Copy initialized data from .sidata (Flash) to .data (RAM)
	memcpy_usr( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
	// Clear the .bss section in RAM.
	memset_usr( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );
}

void main() {


	// Set the core system clock speed.
	// Default clock source is the 8MHz internal oscillator.
	uint32_t SystemCoreClock = 8000000;

	// Enable peripheral clocks and set up GPIO pins.
	// Enable peripheral clocks: GPIOA, USART2.
	RCC_APB1ENR  |=  ( RCC_APB1ENR_USART2EN );
	RCC_APB2ENR  |=  ( RCC_APB2ENR_IOPAEN );
	// Configure pins A2, A3 for USART2.
	GPIOA->CRL    &= ~( GPIO_CRL_MODE2 |
			GPIO_CRL_CNF2 |
			GPIO_CRL_MODE3 |
			GPIO_CRL_CNF3 );
	GPIOA->CRL    |= ( ( 0x1 << GPIO_CRL_MODE2_Pos ) |
			( 0x2 << GPIO_CRL_CNF2_Pos ) |
			( 0x0 << GPIO_CRL_MODE3_Pos ) |
			( 0x1 << GPIO_CRL_CNF3_Pos ) );

	// Set the baud rate
	uint32_t baud_rate = 115200;
	uint16_t uartdiv = SystemCoreClock / baud_rate;
	USART2->BRR = ( ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
			( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	// Enable the USART peripheral.
	USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );

	// Main loop: wait for a new byte, then echo it back.
	//char rxb = '\0';
	putchar('\a'); // beep
	puts(greeting);
	puts("You type, I echo.");
	while ( 1 ) {
		int c = getchar();
		putchar(c);
	}
}
