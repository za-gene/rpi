#include "../stm8.h"

#define TIM2_CR1 *(uchar*)0x005300 // control register
#define TIM2_EGR *(uchar*)0x005306 // Event Generation Register
#define TIM2_CNTRH *(uchar*)0x00530C  // counter high
#define TIM2_CNTRL *(uchar*)0x00530D  // counter low
#define TIM2_PSCR *(uchar*)0x00530E // prescaler

#define TIM2_CR1_CEN (1<<0) // counter enable
#define TIM2_EGR_UG (1<<0) // update generation

#define TIM2_ARRH *(uint8_t*)0x530F // Auto reload register high
#define TIM2_ARRL *(uint8_t*)0x5310 // Auto reload register high
#define TIM2_CCR1H *(uint8_t*)0x5311  // Capture compare register 1 high
#define TIM2_CCR1L *(uint8_t*)0x5312  // Capture compare register 1 low
#define TIM2_CCER1 *(uint8_t*)0x530A // apture compare enable register 1
#define TIM2_CCER1_CC1P (1<<1)
#define TIM2_CCER1_CC1E (1<<0)
#define TIM2_CCMR1 *(uint8_t*)0x5307  // Capture/compare mod register 1



// #define LED (1<<5) // inbuilt led


/*
#define TIM4_ISR                23
void timer4_millis_isr() __interrupt(TIM4_ISR)
{
}
*/


const uint16_t timer2_arr = 999; // PWM with 2kHz frequency
const uint16_t timer2_ccr1 = 249; // 25% duty cycle


int main()
{
	disable_interrupts();

	// As per datasheet of stm8s103f3 PD4 pin is timer 2 channel 1.
	set_bit(PORTD->DDR, 4); // 0b00010000 PD4 is now output
	set_bit(PORTD->CR1, 4); // 0b00010000 PD4 is now pushpull

	TIM2_PSCR = 0x00; // Prescaler = 1

	// Fill 16 bit timer2_arr to two 8 bit registers.
	// MSB register to be filled first.
	TIM2_ARRH = timer2_arr >> 8;
	TIM2_ARRL = timer2_arr & 0x00FF;

	// Fill 16 bit timer2_ccr1 to two 8 bit registers.
	// MSB register to be filled first.
	TIM2_CCR1H = timer2_ccr1 >> 8;
	TIM2_CCR1L = timer2_ccr1 & 0x00FF;

	//set_bit(TIM2_CCER1, TIM2_CCER1_CC1P); // channel 1 active low
	TIM2_CCER1 |= TIM2_CCER1_CC1P;
	//set_bit(TIM2_CCER1, TIM2_CCER1_CC1E); // Enable channel 1 output
	TIM2_CCER1 |= TIM2_CCER1_CC1E;

	// PWM mode 1.
	set_bit(TIM2_CCMR1, 6); // Set output compare mode as 6 (0b110)
	set_bit(TIM2_CCMR1, 5); // So channel 1 will be active while counter
	clear_bit(TIM2_CCMR1, 4);  // is lower than compare value.

	//set_bit(TIM2_CR1, TIM2_CR1_CEN); // Enable counter
	TIM2_CR1 |= TIM2_CR1_CEN; // enable counter

	enable_interrupts();
	while(1); // do nothing forever
}
