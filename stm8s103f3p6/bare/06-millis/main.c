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

#define LED (1<<5)

void main()
{
	//init_uart();
	//init_millis();
	//uart_print("millis\n\r");

	PORTB->DDR |= LED; // 0x00001000 PB5 is now output
	PORTB->CR1 |= LED; // 0x00001000 PB5 is now pushpull

	//u32 ms = millis();
	while (1)
	{
		//if(millis() - ms < 500) continue;
		//char c = uart_getc();
		//uart_putc('.');
		//ms = millis();
		PORTB->ODR &= ~LED; // Toggle PB5 output
	}
}

