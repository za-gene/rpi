#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
//#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>

// lecture at https://www.youtube.com/watch?v=uKwD3JuRWeA
typedef uint32_t u32;


void exti3_isr(void) // standard name
//void EXTI3_IRQHandler(void) // standard name
{
	gpio_toggle(GPIOC, GPIO13);
	//exti_set_trigger(3, EXTI_TRIGGER_FALLING); // corresponding to EXTI3
	exti_reset_request(EXTI3); // pin 3 (of PB3)
}


int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
	gpio_set(GPIOC, GPIO13);

	//while(1);

	//rcc_clock_setup_in_hse_8mhz_out_72mhz();
	//rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO3); // GPIO3 implies EXTI3
	//gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3); // GPIO3 implies EXTI3
	rcc_periph_clock_enable(RCC_SYSCFG);
	//rcc_periph_clock_enable(RCC_AFIO);
	nvic_enable_irq(NVIC_EXTI3_IRQ);
	exti_select_source(EXTI3, GPIOB);
	exti_set_trigger(EXTI3, EXTI_TRIGGER_FALLING); // corresponding to EXTI3
	exti_enable_request(EXTI3);


	//while(1);
	while (1) gpio_toggle(GPIOC, GPIO13);
}
