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

typedef struct {
	uint32_t peri;
	int rcc;
	uint32_t irq;
} timer_t;

extern timer_t timer2;
#define TIMER2 &timer2

timer_t timer2 = { TIM2, RCC_TIM2, NVIC_TIM2_IRQ};


void mal_timer_init(timer_t* timer, double freq);

void mal_timer_init(timer_t* timer, double freq)
{
	rcc_periph_clock_enable(timer->rcc);
	u32 clk_freq = rcc_get_timer_clk_freq(timer->peri);
	timer_set_prescaler(TIM2, clk_freq/1000000-1); // set the CK_CNT (clock counter) freq to 1MHz
	double period = 1000000.0/freq -1.0;
	timer_set_period(timer->peri, period); // twice a second. Equivalent to TIM_ARR(TIM2) = period
	timer_generate_event(timer->peri, TIM_EGR_UG); // send an update to reset timer and apply settings
	//timer_generate_event(TIM2, TIM_EGR_UG | TIM_EGR_TG); // equiv: TIM_EGR(TIM2) |= (TIM_EGR_UG | TIM_EGR_UG)
	//timer_enable_update_event(TIM2); // equiv: TIM_CR1(TIM2) &= ~TIM_CR1_UDIS
	timer_enable_counter(timer->peri); // equiv: TIM_CR1(TIM2) |= TIM_CR1_CEN
	timer_enable_irq(TIM2, TIM_DIER_UIE); // equiv: TIM_DIER(TIM2) |= TIM_DIER_UIE
	nvic_enable_irq(timer->irq);
}

int main(void)
{
	pin_out(PC13);
	double freq = 2.0;
	//freq = 8.0;
	mal_timer_init(TIMER2, freq);


	while(1);
}
