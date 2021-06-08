#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>

typedef uint32_t u32;

#define GPIOn GPIO14

volatile int on =0;
void tim2_isr(void) // the standard ISR name for TIM2
{
#if 1
	if(on) 
		gpio_set(GPIOC, GPIOn);
	else
		gpio_clear(GPIOC, GPIOn);

	on = 1-on;
#else
	gpio_toggle(GPIOC, GPIOn);
#endif
	//TIM_CNT(TIM2) = 1;
	timer_clear_flag(TIM2, TIM_SR_UIF); // clear the interrupt flag so that it can retrigger // seems important
}

int main(void)
{
	// Set up the builtin LED on PC13
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIOn);
	//gpio_set(GPIOC, GPIO13);

	// trigger the isr twice a second
	rcc_periph_clock_enable(RCC_TIM2);
	volatile u32 clk_freq = rcc_get_timer_clk_freq(TIM2);
#if 1
	timer_set_prescaler(TIM2, clk_freq/1000000-1); // set the CK_CNT (clock counter) freq to 1MHz
	double freq = 100.0; //hz
	volatile double period = 1000000.0/freq -1.0;
	timer_set_period(TIM2, period); // twice a second
#else
	timer_set_prescaler(TIM2, 8000);
	timer_set_period(TIM2, 400);
#endif
	/* Set timer start value. */
	//TIM_CNT(TIM2) = 1; // fishy
	//timer_disable_preload(TIM2); //fishy
	//timer_continuous_mode(TIM2); //fishy
	timer_generate_event(TIM2, TIM_EGR_UG);
	timer_generate_event(TIM2, TIM_EGR_TG | TIM_EGR_UG);
	timer_enable_preload(TIM2); // introduced. doesn't seem to help
	timer_enable_counter(TIM2); // seems important
	timer_enable_irq(TIM2, TIM_DIER_UIE); // update interrupt enable // seems unimportant
	//timer_enable_irq(TIM2, TIM_DIER_TIE); // has peculiar behaviour
	nvic_enable_irq(NVIC_TIM2_IRQ); // seems unimportant
	//nvic_setup();

	/* Update interrupt enable. */
	//TIM_DIER(TIM2) |= TIM_DIER_UIE;

	/* Start timer. */
	//TIM_CR1(TIM2) |= TIM_CR1_CEN;
	//timer_enable_counter(TIM2);
	//rcc_periph_reset_pulse(RST_TIM2);

	while(1);
}
