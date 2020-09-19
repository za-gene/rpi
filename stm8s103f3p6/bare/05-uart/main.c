#include "../stm8.h"


//
//  Setup the system clock to run at 16MHz using the internal oscillator.
//
void InitialiseSystemClock()
{
	CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
	CLK_ICKR |= CLK_ICKR_HSIEN ;                 //  Enable the HSI.
	CLK_ECKR = 0;                       //  Disable the external clock.
	while ((CLK_ICKR & CLK_ICKR_HSIRDY) == 0);       //  Wait for the HSI to be ready for use.
	CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
	CLK_PCKENR1 = 0xff;                 //  Enable all peripheral clocks.
	CLK_PCKENR2 = 0xff;                 //  Ditto.
	CLK_CCOR = 0;                       //  Turn off CCO.
	CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
	CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
	CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
	CLK_SWCR = 0;                       //  Reset the clock switch control register.
	CLK_SWCR |= CLK_SWCR_SWEN;                  //  Enable switching.
	while ((CLK_SWCR & CLK_SWCR_SWBSY) != 0);        //  Pause while the clock switch is busy.
}


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
	/*
	   UART1_CR1_M = 0;        //  8 Data bits.
	   UART1_CR1_PCEN = 0;     //  Disable parity.
	   UART1_CR3_STOP = 0;     //  1 stop bit.
	   */
	UART1_BRR2 = 0x0a;      //  given in original exampl3
	UART1_BRR2 = 0x0b;      //  Set the baud rate registers to 115200 baud
	UART1_BRR1 = 0x08;      //  based upon a 16 MHz system clock.

	// assuming that the default clock is 2MHz...


	//
	//  Disable the transmitter and receiver.
	//
	//UART1_CR2_TEN = 0;      //  Disable transmit.
	//UART1_CR2_REN = 0;      //  Disable receive.
	//
	//  Set the clock polarity, lock phase and last bit clock pulse.
	//
	UART1_CR3 |= UART1_CR3_CPOL;
	UART1_CR3 |= UART1_CR3_CPHA;
	UART1_CR3 |= UART1_CR3_LBCL;
	//
	//  Turn on the UART transmit, receive and the UART clock.
	//
	UART1_CR2 |= UART1_CR2_TEN;
	UART1_CR2 |= UART1_CR2_REN;
	UART1_CR3 |= UART1_CR3_CLKEN;
}


void uart_putc(char c)
{
	while ((UART1_SR & UART1_SR_TXE)==0); //  Wait for transmission complete
	UART1_DR = c; //  Put next char data transmission reg
}

//
//  Send a message to the debug port (UART1).
//
void UARTPrintf(char *message)
{
	char *ch = message;
	while (*ch)
		uart_putc(*ch++);
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
	UARTPrintf("Uart example\n\r");
	while (1)
	{
		UARTPrintf("Hello from my microcontroller....\n\r");
		for (long counter = 0; counter < 2500000; counter++);
	}
}

