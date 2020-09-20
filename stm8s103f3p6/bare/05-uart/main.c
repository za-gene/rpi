#include "../stm8.h"


// set uart to 115200 baud, assuming clock speed is 2MHz
void init_uart()
{
	// set baud rate (see notes)
	UART1_BRR2 = 0x01;
	UART1_BRR1 = 0x01;

	UART1_CR2 |= UART1_CR2_TEN; // enable transmit
	UART1_CR2 |= UART1_CR2_REN; // enable receive
}


char uart_getc()
{
	while(!(UART1_SR & UART1_SR_RXNE)); //  Block until char rec'd
	return  UART1_DR;
}

void uart_putc(char c)
{
	while(!(UART1_SR & UART1_SR_TXE)); //  Wait for transmission complete
	UART1_DR = c; //  transmit char
}

void uart_print(char *msg)
{
	while (*msg) uart_putc(*msg++);
}

void main()
{
	init_uart();
	uart_print("Uart example: you type, I echo\n\r");
	while (1)
	{
		char c = uart_getc();
		uart_putc(c);
	}
}

