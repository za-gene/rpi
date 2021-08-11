#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <math.h>

#include "mcp4921-dma.h"
#include "pi.h"

#define HARE 16

double framerate = 44100, saw_freq = 500;
uint64_t period = 1'000'000/framerate;


volatile float y = 0, dy;

static void alarm_0_irq() 
{
	pi_alarm_rearm(0, period);
	mcp4921_dma_put(y);
	y += dy;
	if(y>4095) y=0;
}

int main() 
{
	stdio_init_all();
	pi_gpio_init(HARE, OUTPUT);
	dy = 4095 * saw_freq / framerate;
	mcp4921_dma_init();
	pi_alarm_init(0, alarm_0_irq, period);

	while(1);
	return 0;
}

