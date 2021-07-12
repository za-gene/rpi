//#include <libopencm3/stm32/rcc.h>
//#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/stm32/timer.h>
//#include <libopencm3/stm32/spi.h>

#include "mal.h"

typedef uint32_t u32;

#define LED PC13

int main(void)
{
	pin_out(LED);

	while (1)
	{
		pin_toggle(LED);
		mal_delayish(100);
	}
}
