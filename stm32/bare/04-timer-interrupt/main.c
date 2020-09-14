#include <stdio.h>
#include <stdbool.h>

#include <blue.h>


// section 10.1.2 Vector table

#define NVIC_TIM4 *(volatile uint32_t *)(0x000000B8)



// section 15.4.4 TIMx DMA/Interrupt enable register (TIMx_DIER)
#define TIM_DIER_UIE (1<<0)
#define TIM_DIER_TIE (1<<6)

#define TIM_EGR_UG (1<<0)
#define TIM_EGR_TG (1<<6)

//void __attribute__ ((interrupt ("IRQ"))) myhandler()
void TIM4_IRQHandler()
{
	puts("hi");
	//TIM4->EGR |= TIM_EGR_UG; // send an update even to reset timer and apply settings
	//TIM4->SR &= ~0x01; // clear UIF
	TIM4->DIER |= 0x01; // UIE
}

void setup_timer()
{
	RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC=7999;
	TIM4->ARR=1000;
	TIM4->EGR |= TIM_EGR_UG; // send an update even to reset timer and apply settings
	TIM4->EGR |= (TIM_EGR_TG | TIM_EGR_UG);
	TIM4->DIER |= 0x01; // UIE enable interrupt
	TIM4->CR1 |= TIM_CR1_CEN;
	puts("Timer setup");
}

void main() 
{
	init_serial();
	puts("04-timer-interrupt started 4");
	char msg[40];

	setup_timer();

	//NVIC_TIM4 = (uint32_t) myhandler; // seems to cause a problem
	//TIM4->DIER |= (TIM_DIER_UIE | TIM_DIER_TIE);
	puts("Interrupt set");


	asm("CPSIE I"); // enable interrupts

	putchar('\a'); // beep

	int secs = 0;
	while(1) {
		itoa(secs++, msg, 10);
		puts(msg);
		for(int i=0; i< 1000000; i++);
		//delay(1000);
	}

	//while(1);
}

void SystemInit()
{
	// TODO
}

void __libc_init_array()
{
	// TODO
	
}
