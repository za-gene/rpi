//#include <stdio.h>
//#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/spi.h"
//#include "hardware/gpio.h"


//#define BTN 14 // GPIO number, not physical pin
//#define LED 25 // GPIO of built-in LED

#if 0
void freq_input_rise_isr(uint gpio, uint32_t events)
{
	static uint32_t cnt = 0;
	uint32_t factor = 1;
	for(int i = 1; i < 11; i++) {
		factor <<= 1;
		//flip(i, factor);
		gpio_put(i, (cnt % factor) >= (factor/2));
	}
	cnt++;
}
#endif

int main() 
{
	//stdio_init_all();

#if 0
	for(int i = 1; i < 11; i++) {
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
	}
#endif
	const uint LED = PICO_DEFAULT_LED_PIN;
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	gpio_put(LED, 1);
	//gpio_set_irq_enabled_with_callback(0, GPIO_IRQ_EDGE_RISE, true, &freq_input_rise_isr);
	//gpio_init(BTN);
	//gpio_set_dir(BTN, GPIO_IN);
	//gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	/*
	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	for(;;) {
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);
	}
	*/

	//gpio_put(1, 1);
	while(1) sleep_ms(250);

	//return 0;
}

