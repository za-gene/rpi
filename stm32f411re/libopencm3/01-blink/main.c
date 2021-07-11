#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

typedef uint32_t u32;



/** @brief Delay for approx time measured in ms
Calibrated for an STMF411
*/

void delayish(uint32_t ms);

void __attribute__((optimize("O0"))) delayish(uint32_t ms)
{
#define APPROX_1MS 717
	for(uint32_t j=0; j<ms; ++j) {
		for (int i = 0; i < APPROX_1MS; i++) {
			__asm__("nop");
		}
	}
}

int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
	gpio_set(GPIOC, GPIO13);


	while (1)
	{
		delayish(100);
		gpio_toggle(GPIOC, GPIO13);
	}
}
