/*
 * Calibrating the delayish() function. The aim here is
 * to use nop() to create a delay of approx 1ms without
 * using timers.
 *
 * A lot of this was cobbled from project 04
 */

#include <usart.h>
#include <blue.h>



// section 15.4.4 TIMx DMA/Interrupt enable register (TIMx_DIER)
#define TIM_DIER_UIE (1<<0)
#define TIM_DIER_TIE (1<<6)

#define TIM_EGR_UG (1<<0)
#define TIM_EGR_TG (1<<6)

u32 ticks = 0;

void TIM4_IRQHandler()
{
	ticks++;
	//gpio_toggle(BUILTIN_LED);
	//puts("hi");
	//TIM4->EGR |= TIM_EGR_UG; // send an update even to reset timer and apply settings
	TIM4->SR &= ~0x01; // clear UIF
	//TIM4->DIER |= 0x01; // UIE
}

void setup_timer()
{
	RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
	//TIM4->PSC=7199;
	TIM4->PSC=7999;
	TIM4->ARR=1000;
	//TIM4->ARR=100; // fiddle around for testing purposes
	TIM4->EGR |= TIM_EGR_UG; // send an update even to reset timer and apply settings
	TIM4->EGR |= (TIM_EGR_TG | TIM_EGR_UG);
	TIM4->DIER |= 0x01; // UIE enable interrupt
	TIM4->CR1 |= TIM_CR1_CEN;
	puts("Timer setup");
}

#define NVIC_ISER0 *(volatile uint32_t *)(0xE000E000+0x100)

int main() {
	init_serial();
	puts("10-delayish 1");
	setup_timer();
	NVIC_ISER0 = (1<<30);
	TIM4->DIER |= 1;

	enable_irq();
	puts("Interrupt set");


	char str[20];
	puts("Start");
	u32 start = ticks, elapsed;
	delaysecsish(60);
	elapsed = ticks -start;
	/*
	while(1) {
		elapsed = ticks -start;
		if(elapsed>=100UL) break;
	}
	*/
	puts("Stop");
	itoa(elapsed, str, 10);
	puts(str);


	while(1);
}

