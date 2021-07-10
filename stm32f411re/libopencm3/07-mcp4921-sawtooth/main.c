#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "mal.h"


typedef uint32_t u32;

#define LED PC13

void tim2_isr(void) // the standard ISR name for TIM2
{
	timer_clear_flag(TIM2, TIM_SR_UIF); // hmmm, seems to matter that it's at the top
	pin_toggle(LED);
}


int main(void)
{
	pin_out(PC13);
	double freq = 2.0;
	freq = 8.0;
	mal_timer_init(TIMER2, freq);


	while(1);
}
