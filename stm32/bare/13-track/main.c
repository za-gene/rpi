#include <gpio.h>
#include <timers.h>
#include <usart.h>

#include "track.h"


#define FREQ 440
#define NSAMPLES 16


char msg[40];
void printi(u32 v) 
{
	itoa(v, msg, 10);
	print(msg);
}
void print_bin(u32 v)
{
	for(int i=0; i<8; i++) {
		print(i==0 ? " 0b" : "'");
		for(int j=0; j<4; j++) {
			putchar(v & (1<<31) ? '1' : '0');
			v <<= 1;
		}
	}
}




void TIM3_IRQHandler()
{
	// increment the PWM counter for when to set low
	static u32 idx=0;
	if(idx >= 2*track_raw_len) idx = 0;
	TIM3->CCR1 = (u32) track_raw[idx/2];
	idx++;

	TIM3->SR &= ~TIM_SR1_UIF; // reset interrupt flag
}


/* freq in Hz
 * duty_pc as a percentage
 *
 * It works specifically on PA6, which is timer 3, channel 1.
 *
 * RCC_APB2ENR_AFIOEN apparently not required.
 */

void setup_timer()
{
	RCC_APB1ENR |= RCC_APB1ENR_TIM3EN;
	gpio_mode(PA6, 0b1011); // output 50MHz, push-pull, alt function

	TIM3->ARR = 255;
	//TIM3->PSC = 8000000UL/FREQ/NSAMPLES/NSAMPLES ;
	TIM3->PSC = 7;
	printi(TIM3->PSC);


	TIM3->CCER |= TIM_CCER_CC1E; // enable capture/compare 
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE; // enable preload
	TIM3->CCMR1 |= 0b110<<4; // output pwm compare mode 1 
	TIM3->CR1 |= TIM_CR1_CEN; // enable counter
}


void printn(u32 v)
{
	printi(v);
	print_bin(v);
	puts("");
}


#define RCC_CR_PLLON (1<<24)
#define RCC_CR_PLLRDY (1<<25)

void main() 
{
	//RCC_CFGR |= 0b0101 << 24; // MCO to PLL
	RCC_CFGR |= 0b0110 << 18; // PLLMUL 8X
	RCC_CFGR |= 0b10;  //PLL selected as system clock
	while(RCC_CR & RCC_CR_PLLRDY);
	RCC_CR |= RCC_CR_PLLON;

	gpio_mode_out(BUILTIN_LED);
	init_serial();
	puts("12 pwm with interrupt");

	setup_timer();

	// setup interrupt
	NVIC_ISER0 |= (1<<29); // position for TIM3_IRQHandler
	TIM3->DIER |= TIM_DIER_UIE; // enable interrupt (this wasn't in project 11)
	enable_irq();

	//printn(TIM3->PSC);
	//printn(TIM3->ARR);
	beep();

	while(1);
}

