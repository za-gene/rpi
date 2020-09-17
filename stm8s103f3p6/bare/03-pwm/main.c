#include "../stm8.h"

/* frequency calc
 * prescaler 0b1111 (the max) is 2^15, implying frequency of 2MHz/2^15 = 61.03Hz
 * to get a frequency of of 1Hz therefore divide by 60
 * calcs probably contain off-by-one erros
 */

const uint16_t timer2_arr = 60; // frequency
const uint16_t timer2_ccr1 = timer2_arr/2; // 50% duty cycle


int main()
{
	disable_interrupts();

	// As per datasheet of stm8s103f3 PD4 pin is timer 2 channel 1.
	set_bit(PORTD->DDR, 4); // 0b00010000 PD4 is now output
	set_bit(PORTD->CR1, 4); // 0b00010000 PD4 is now pushpull

	//TIM2_PSCR = 0x00; // Prescaler = 1
	TIM2_PSCR = 0b1111; // Prescaler = 32768

	// Fill 16 bit timer2_arr to two 8 bit registers.
	// MSB register to be filled first.
	TIM2_ARRH = timer2_arr >> 8;
	TIM2_ARRL = timer2_arr & 0x00FF;

	// Fill 16 bit timer2_ccr1 to two 8 bit registers.
	// MSB register to be filled first.
	TIM2_CCR1H = timer2_ccr1 >> 8;
	TIM2_CCR1L = timer2_ccr1 & 0x00FF;

	TIM2_CCER1 |= TIM2_CCER1_CC1P;a // channel 1 active low
	TIM2_CCER1 |= TIM2_CCER1_CC1E; // enable channel 1 output

	// PWM mode 1.
	set_bit(TIM2_CCMR1, 6); // Set output compare mode as 6 (0b110)
	set_bit(TIM2_CCMR1, 5); // So channel 1 will be active while counter
	clear_bit(TIM2_CCMR1, 4);  // is lower than compare value.

	TIM2_CR1 |= TIM2_CR1_CEN; // enable counter

	enable_interrupts();
	while(1); // do nothing forever
}
