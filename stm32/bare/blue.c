#include "blue.h"

int putchar(int c)
{
	while( !( USART2->SR & USART_SR_TXE ) ) {};
	USART2->DR = c;
	return c;
}

int puts(const char* s)
{
	while(s && *s) putchar(*s++);
	putchar('\r');
	putchar('\n');
	return 0;
}

