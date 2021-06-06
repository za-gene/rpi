#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

typedef uint32_t u32;


int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
	gpio_set(GPIOC, GPIO13);


#define LITTLE_BIT 10000
	while (1)
	{
		/* wait a little bit */
		for (int i = 0; i < LITTLE_BIT; i++)
		{
			__asm__("nop");
		}
		gpio_toggle(GPIOC, GPIO13);
	}
}