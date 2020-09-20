#include "../stm8.h"

/*
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
*/

#define LED (1<<5)

ulong dl;

#if 0
volatile uint32_t __global_millis = 0;

void timer4_millis_isr() __interrupt(TIM4_ISR) 
{
	__global_millis++;
	//clear_bit(TIM4_SR, TIM4_SR_UIF);  // Clear interrupt flag
	TIM4_SR &= ~TIM4_SR_UIF;
}

void init_millis()
{
	enable_interrupts();
	TIM4_PSCR = 3; // So prescaler will be 2^3 = 8
	// Timer 4 will increment in each 4 micro seconds
	TIM4_ARR = 249; // So timer 4 will overflow in each milli second.
	//set_bit(TIM4_IER, TIM4_IER_UIE);  // Enable update interrupt.
	TIM4_IER |= TIM4_IER_UIE;
	//set_bit(TIM4_EGR, TIM4_EGR_UG);
	//set_bit(TIM4_CR1, TIM4_CR1_CEN);  // Enable timer 4.
	TIM4_CR1 |= TIM4_CR1_CEN;
}

/**
 * Return current milli seconds.
 */
uint32_t millis()
{
	return __global_millis;
}
#endif
void main()
{
	//CLK_CKDIVR = 0x00; // set clock to 16MHz
	//PORTB->ODR = 0;
	//init_uart();
	init_millis();
	//uart_print("millis\n\r");

	PORTB->DDR |= LED; // 0x00001000 PB5 is now output
	PORTB->CR1 |= LED; // 0x00001000 PB5 is now pushpull

	u32 ms = millis();
	while (1)
	{
		if(millis() - ms > 1000) {
			//continue;
		//char c = uart_getc();
		//uart_putc('.');
		ms = millis();
		PORTB->ODR ^= LED; // Toggle PB5 output
		//for(dl=0; dl<200000UL ; dl++);
		}
	}
}

