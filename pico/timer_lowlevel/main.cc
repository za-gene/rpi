#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/gpio.h"

#include "pi.h"

#define DELAY (2*1'000'000)

/// \tag::get_time[]
// Simplest form of getting 64 bit time from the timer.
// It isn't safe when called from 2 cores because of the latching
// so isn't implemented this way in the sdk
static uint64_t get_time(void) {
	// Reading low latches the high value
	uint32_t lo = timer_hw->timelr;
	uint32_t hi = timer_hw->timehr;
	return ((uint64_t) hi << 32u) | lo;
}
/// \end::get_time[]




static void alarm_0_irq() 
{
	// Assume alarm 0 has fired
	pi_alarm_rearm(0, DELAY);
	printf("Alarm IRQ fired\n");
}


int main() {
	stdio_init_all();
	printf("Timer lowlevel!\n");

	pi_alarm_init(0, alarm_0_irq, DELAY);
	while(1);

}

