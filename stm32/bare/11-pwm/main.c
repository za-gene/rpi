#include <gpio.h>
#include <timers.h>
#include <usart.h>



char msg[40];
void printi(u32 v) 
{
	itoa(v, msg, 10);
	print(msg);
	//puts(msg);
	//for(int i=0; i< 1000000; i++);
	//delay(1000);
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




void setup_timer()
{
	RCC_APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN; // dunno if needed
	gpio_mode(PA6, 0b1011); // output, push-pull, alt function
	u32 freq = 440;
	u32 arr = freq;
       	u32 psc = 8000000/freq/arr; // based on system core clock of 8MHz
	while(psc >= (1<<16)) {
		psc = (psc>>1);
		arr = (arr<<1);
	}
	TIM3->PSC = psc -1;
	TIM3->ARR = arr -1;
	//u32 rescale = 10; // we need this so both psc and arr are 16-bits
	//TIM3->PSC=72000000UL/freq/rescale -1; 
	//TIM3->ARR=freq*rescale-1;
	//TIM3->PSC = 7199;
	//TIM3->ARR = 999;
	TIM3->CCR1 = arr/4 -1; // duty cycle 25% (1/4)
	TIM3->CCER |= TIM_CCER_CC1E; // enable capture/comapre 
	TIM3->CCMR1 |= 0b110<<4; // output pwm compare mode 1 
	//TIM3->EGR |= TIM_EGR_UG;;
	TIM3->CR1 |= TIM_CR1_CEN; // enable counter

	//TIM4->ARR=100; // fiddle around for testing purposes
	//TIM4->EGR |= TIM_EGR_UG; // send an update even to reset timer and apply settings
	TIM3->EGR |= (TIM_EGR_TG | TIM_EGR_UG);
	//TIM4->DIER |= 0x01; // UIE enable interrupt
	TIM3->CR1 |= TIM_CR1_CEN;
	puts("Timer setup");
}


void main() 
{
	init_serial();
	puts("11 pwm");

	setup_timer();
	//gpio_mode_out(BUILTIN_LED);

	//NVIC_ISER0 = (1<<30);
	//TIM4->DIER |= 1;
	printi(TIM3->PSC);
	print_bin(TIM3->PSC);
	puts("");
	printi(TIM3->ARR);
	print_bin(TIM3->ARR);

	//enable_irq();

	putchar('\a'); // beep

	int secs = 0;
	while(1) {
	}

	//while(1);
}

