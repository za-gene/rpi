#include <stdio.h>
#include <stdbool.h>

#include <gpio.h>


void delay(uint16_t ms)
{
	static bool initialised = false;
	if(!initialised) {
		// timer setup
		RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
		TIM4->PSC=7999;
		TIM4->ARR=65535;
		TIM4->CR1 |= TIM_CR1_CEN;
		initialised = true;
	}

	/* According to https://bit.ly/2GXlmxd 
	 * you must define start and now as 32-bit integers rather 
	 * than 16- or 8-,
	 * otherwise the bridge creates duplications
	 */
	uint32_t start = TIM4->CNT;
	while(1) {
		uint32_t now = TIM4->CNT;
		if(now-start>=ms) break;
	}
}


void main() 
{
	gpio_mode_out(BUILTIN_LED);
	init_serial();
	puts("03-delay started");
	char msg[40];

	putchar('\a'); // beep
	int secs = 0;
	while(1) {
		gpio_toggle(BUILTIN_LED);
		itoa(secs++, msg, 10);
		puts(msg);
		delay(1000);
	}
}
