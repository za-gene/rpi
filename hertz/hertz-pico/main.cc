#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
//#include <atomic>
//#include <stdatomic.h>

using  u64 = uint64_t;
using  u32 = uint32_t;
using  u16 = uint16_t;


u32 counter = 0; // I'm reasonably confident this is atomic

// On GP2 (physical pin 4)
void gpio_callback(uint gpio, uint32_t events) 
{
	counter++;
}

bool repeating_timer_callback(struct repeating_timer *t) {
	u32 tmp = counter;
	counter = 0;
	printf("%d\n", tmp);
	return true;
}


int main() 
{
	stdio_init_all();
	printf("Hertz\n");
	gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);


	struct repeating_timer timer;
	add_repeating_timer_ms(-1000, repeating_timer_callback, NULL, &timer);

	for(;;);

	return 0;
}

