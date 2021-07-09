#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>
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

	// trigger the isr twice a second
	rcc_periph_clock_enable(RCC_TIM2);
	u32 clk_freq = rcc_get_timer_clk_freq(TIM2);
	timer_set_prescaler(TIM2, clk_freq/1000000-1); // set the CK_CNT (clock counter) freq to 1MHz
	double freq = 2.0; //hz 
	freq = 8.0;
	double period = 1000000.0/freq -1.0;
	timer_set_period(TIM2, period); // twice a second. Equivalent to TIM_ARR(TIM2) = period
	timer_generate_event(TIM2, TIM_EGR_UG); // send an update to reset timer and apply settings
	//timer_generate_event(TIM2, TIM_EGR_UG | TIM_EGR_TG); // equiv: TIM_EGR(TIM2) |= (TIM_EGR_UG | TIM_EGR_UG)
	//timer_enable_update_event(TIM2); // equiv: TIM_CR1(TIM2) &= ~TIM_CR1_UDIS
	timer_enable_counter(TIM2); // equiv: TIM_CR1(TIM2) |= TIM_CR1_CEN
	timer_enable_irq(TIM2, TIM_DIER_UIE); // equiv: TIM_DIER(TIM2) |= TIM_DIER_UIE
	nvic_enable_irq(NVIC_TIM2_IRQ);

	while(1);
}
