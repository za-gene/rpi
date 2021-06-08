#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>

typedef uint32_t u32;

#define GPIOn GPIO13

void tim2_isr(void) // the standard ISR name for TIM2
{
	timer_clear_flag(TIM2, TIM_SR_UIF); // hmmm, seems to matter that it's at the top
	gpio_toggle(GPIOC, GPIOn);
}

int main(void)
{
	// Set up the builtin LED on PC13
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIOn);

	// trigger the isr twice a second
	rcc_periph_clock_enable(RCC_TIM2);
	u32 clk_freq = rcc_get_timer_clk_freq(TIM2);
	timer_set_prescaler(TIM2, clk_freq/1000000-1); // set the CK_CNT (clock counter) freq to 1MHz
	double freq = 2.0; //hz 
	double period = 1000000.0/freq -1.0;
	timer_set_period(TIM2, period); // twice a second
	timer_enable_update_event(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
	timer_enable_counter(TIM2);
	nvic_enable_irq(NVIC_TIM2_IRQ);

	while(1);
}
