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
	//unsigned char tmp = UART1_SR;
	//tmp = UART1_DR;

	//UART1_SR = 0xC0; // mcarter set to default value

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

	// clear certain bits
	UART1_CR1 &= ~UART1_CR1_M ;        //  8 Data bits.
	UART1_CR1 &= ~UART1_CR1_PCEN;     //  Disable parity

	// stop bits
	UART1_CR3 &= 0b11001111; // unmask the stop bit to default (1 stop bit)
	//UART1_CR3 |= 0b00100000; // two stop bits
	//UART1_CR3 |= 0b00110000; // 1.5 stop bits
	//UART1_CR3 &= ~UART1_CR3_STOP;     //  1 stop bit.

#if 1 //115200 baud
	//UART1_BRR2 = 0x0a;      //  given in original example
	UART1_BRR2 = 0x0b;      //  Set the baud rate registers to 115200 baud
	UART1_BRR1 = 0x08;      //  based upon a 16 MHz system clock.
#else // 9600 baud, but seems to be worse than 115200
	UART1_BRR2 = 0x03;
	UART1_BRR1 = 0x69;
#endif

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
	//UART1_CR3 |= UART1_CR3_LBCL; // this seems to cause problems

	UART1_CR2 |= UART1_CR2_TEN; // enable transmit
	UART1_CR2 |= UART1_CR2_REN; // enable receive
	UART1_CR3 |= UART1_CR3_CLKEN; // unable uart clock
}


char uart_getc()
{
	while((UART1_SR & UART1_SR_RXNE)==0); //  Block until char rec'd
	//char c =  UART1_DR;
	//return c;
	return UART1_DR;
}

void uart_putc(char c)
{
	while((UART1_SR & UART1_SR_TXE)==0); //  Wait for transmission complete
	UART1_DR = c; //  transmit char
}

void UARTPrintf(char *message)
{
	char *ch = message;
	while (*ch)
		uart_putc(*ch++);
}

void main()
{
	disable_interrupts();
	InitialiseSystemClock();
	init_uart();
	enable_interrupts();
	UARTPrintf("Uart example: you type, I echo\n\r");
	while (1)
	{
		//continue;
		char c = uart_getc();
		uart_putc(c);
		//UARTPrintf("Hello from my microcontroller....\n\r");
		//for (long counter = 0; counter < 2500000; counter++);
	}
}

