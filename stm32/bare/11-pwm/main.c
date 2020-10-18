#include <gpio.h>
#include <timers.h>
#include <usart.h>



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



/* freq in Hz
 * duty_pc as a percentage
 */

void setup_timer(u32 freq, u32 duty_pc)
{
	RCC_APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN; // dunno if needed
	gpio_mode(PA6, 0b1011); // output 50MHz, push-pull, alt function

	// figure out prescaler and array
	// Must fit in a u16, even though 32-bit wide
	u32 arr = freq;
       	u32 psc = 8000000/freq/arr; // based on system core clock of 8MHz
	while(psc >= (1<<16)) {
		psc = (psc>>1);
		arr = (arr<<1);
	}
	TIM3->PSC = psc -1;
	TIM3->ARR = arr -1;
	TIM3->CCR1 = arr * duty_pc / 100 -1;
	TIM3->CCER |= TIM_CCER_CC1E; // enable capture/compare 
	TIM3->CCMR1 |= 0b110<<4; // output pwm compare mode 1 
	TIM3->CR1 |= TIM_CR1_CEN; // enable counter

	TIM3->EGR |= (TIM_EGR_TG | TIM_EGR_UG);
	TIM3->CR1 |= TIM_CR1_CEN;
}


void printn(u32 v)
{
	printi(v);
	print_bin(v);
	puts("");
}

void main() 
{
	init_serial();
	puts("11 pwm");

	setup_timer(440, 35);

	printn(TIM3->PSC);
	printn(TIM3->ARR);
	beep();

	while(1);
}

