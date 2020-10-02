#include <stdbool.h>

#include "gpio.h"

#define _USART USART1 // the default USART we're going to use

void usart_2()
{
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
}

void usart_1()
{
	RCC_APB2ENR  |=  RCC_APB2ENR_USART1EN;
	//RCC_APB2ENR  |=  RCC_APB2ENR_IOPAEN;
	gpio_mode(PA9, OUTPUT_ALT);
	gpio_mode(PA10, INPUT);

}

void init_serial()
{

	// Set the core system clock speed.
	// Default clock source is the 8MHz internal oscillator.
	uint32_t SystemCoreClock = 8000000;

	usart_1();

	// Set the baud rate
	uint32_t baud_rate = 115200;
	uint16_t uartdiv = SystemCoreClock / baud_rate;
	_USART->BRR = ( ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
			( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	// Enable the USART peripheral.
	_USART->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );
}

int getchar()
{
	while( !( _USART->SR & USART_SR_RXNE ) ) {};
	return _USART->DR;
}


int putchar(int c)
{
	while( !( _USART->SR & USART_SR_TXE ) ) {};
	_USART->DR = c;
	return c;
}


int puts(const char* s)
{
	while(s && *s) putchar(*s++);
	putchar('\r');
	putchar('\n');
	return 0;
}

/* reverse() and itoa() from https://www.geeksforgeeks.org/implement-itoa/
*/


/* A utility function to reverse a string  */
void reverse(char str[], int length) 
{ 
	int start = 0; 
	int end = length -1; 
	while (start < end) 
	{ 
		//swap(*(str+start), *(str+end)); 
		int c = *(str+start);
		*(str+start) = *(str+end);
		*(str+end) = c;

		start++; 
		end--; 
	} 
} 

// Implementation of itoa() 
char* itoa(int num, char* str, int base) 
{ 
	int i = 0; 
	bool isNegative = false; 

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) 
	{ 
		str[i++] = '0'; 
		str[i] = '\0'; 
		return str; 
	} 

	// In standard itoa(), negative numbers are handled only with  
	// base 10. Otherwise numbers are considered unsigned. 
	if (num < 0 && base == 10) 
	{ 
		isNegative = true; 
		num = -num; 
	} 

	// Process individual digits 
	while (num != 0) 
	{ 
		int rem = num % base; 
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
		num = num/base; 
	} 

	// If number is negative, append '-' 
	if (isNegative) 
		str[i++] = '-'; 

	str[i] = '\0'; // Append string terminator 

	// Reverse the string 
	reverse(str, i); 

	return str; 
} 
