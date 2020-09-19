#include "../stm8.h"



//
//  Setup the UART to run at 115200 baud, no parity, one stop bit, 8 data bits.
//
//  Important: This relies upon the system clock being set to run at 16 MHz.
//
void init_uart()
{
	//
	//  Clear the Idle Line Detected bit in the status register by a read
	//  to the UART1_SR register followed by a Read to the UART1_DR register.
	//
	unsigned char tmp = UART1_SR;
	tmp = UART1_DR;
	//
	//  Reset the UART registers to the reset values.
	//
	UART1_CR1 = 0;
	UART1_CR2 = 0;
	UART1_CR4 = 0;
	UART1_CR3 = 0;
	UART1_CR5 = 0;
	UART1_GTR = 0;
	UART1_PSCR = 0;
	//
	//  Now setup the port to 115200,n,8,1.
	//
	UART1_CR1_M = 0;        //  8 Data bits.
	UART1_CR1_PCEN = 0;     //  Disable parity.
	UART1_CR3_STOP = 0;     //  1 stop bit.
	UART1_BRR2 = 0x0a;      //  Set the baud rate registers to 115200 baud
	UART1_BRR1 = 0x08;      //  based upon a 16 MHz system clock.
	//
	//  Disable the transmitter and receiver.
	//
	UART1_CR2_TEN = 0;      //  Disable transmit.
	UART1_CR2_REN = 0;      //  Disable receive.
	//
	//  Set the clock polarity, lock phase and last bit clock pulse.
	//
	UART1_CR3_CPOL = 1;
	UART1_CR3_CPHA = 1;
	UART1_CR3_LBCL = 1;
	//
	//  Turn on the UART transmit, receive and the UART clock.
	//
	UART1_CR2_TEN = 1;
	UART1_CR2_REN = 1;
	UART1_CR3_CKEN = 1;
}


//
//  Send a message to the debug port (UART1).
//
void UARTPrintf(char *message)
{
	char *ch = message;
	while (*ch)
	{
		UART1_DR = (unsigned char) *ch;     //  Put the next character into the data transmission register.
		while (UART1_SR_TXE == 0);          //  Wait for transmission to complete.
		ch++;                               //  Grab the next character.
	}
}

//
//	Main program loop.
//
void main()
{
	disable_interrupts();
	InitialiseSystemClock();
	init_uart();
	enable_interrupts();
	while (1)
	{
		UARTPrintF("Hello from my microcontroller....\n\r");
		for (long counter = 0; counter < 250000; counter++);
	}
}

