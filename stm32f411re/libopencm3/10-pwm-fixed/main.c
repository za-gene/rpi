#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/stm32/timer_common_all.h>
#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>

#include "mal.h"

typedef uint32_t u32;

#define LED PC13
const uint32_t timer_peri = TIM1; // timer peripheral
const enum tim_oc_id oc_id = TIM_OC4; // output compare channel designator
const uint32_t period = 1000; // counter in clock ticks

int main(void)
{
	pin_out(LED);

	// setup PA11 for PWM
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_TIM1); // enable TIM1 clock
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11); // pin PA11 Alt Function
	gpio_set_af(GPIOA, GPIO_AF1, GPIO11);

	//timer_reset(timer_peri);
	timer_enable_preload(timer_peri); // causes counter to be loaded from its ARR only at next update event
	timer_enable_oc_preload(timer_peri, oc_id);
	timer_set_counter(timer_peri, period * 1/4); // set duty cycle to 25%
	timer_set_oc_mode(timer_peri, oc_id, TIM_OCM_PWM1); // output active when counter is lt compare register
	timer_set_oc_value(timer_peri, oc_id, period * 1/4); // set duty cycle to 25%
	timer_enable_oc_output(timer_peri, oc_id); // enable timer output compare
	timer_set_period(timer_peri, period); // set the timer period in the auto-reload register
	//timer_set_prescaler();
	timer_continuous_mode(timer_peri); // enable the timer to run continuously
	timer_generate_event(timer_peri, TIM_EGR_UG);
	timer_enable_counter(timer_peri);
	timer_enable_irq(timer_peri, TIM_DIER_COMIE);  //enable commutation interrupt
	nvic_enable_irq(NVIC_TIM1_CC_IRQ);

	while (1)
	{
		pin_toggle(LED);
		mal_delayish(100);
	}
}
