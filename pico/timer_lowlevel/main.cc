#include <stdio.h>
#include "pico/stdlib.h"

#include "pi.h"

#define ALARM 0
#define DELAY (2*1'000'000)


static int i =0;

static void alarm_0_irq() 
{
	pi_alarm_rearm(ALARM, DELAY);
	printf("Alarm IRQ fired %d\n", i++);
}


int main() {
	stdio_init_all();
	printf("Timer lowlevel!\n");

	pi_alarm_init(ALARM, alarm_0_irq, DELAY);
	while(1);
}
