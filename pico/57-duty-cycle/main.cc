#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#include "pi.h"
#include "ssd1306.h"

typedef uint32_t u32;

#define ALARM 0
#define DELAY (1'000'000/ 10'000)

#define DUTY_PIN 27

bool do_counting = true;
u32 ticks = 0, ups = 0;
static void alarm_0_irq() 
{
	pi_alarm_rearm(ALARM, DELAY);
	if(!do_counting) return;
	if(gpio_get(DUTY_PIN)) ups++;
	ticks++;
}


int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	init_display(64, 4);
	pi_gpio_init(DUTY_PIN, INPUT);
	pi_alarm_init(ALARM, alarm_0_irq, DELAY);

	char msg[100];
	for(;;) {
		do_counting = false;
		double duty = (double)ups * 100.0 / (double) ticks;
		clear_scr();
		setCursorx(0);		
		setCursory(0);
		sprintf(msg, "DUTY: %.1f%%", duty);
		ssd1306_puts(msg);;
		show_scr();
		ticks = 0;
		ups = 0;
		do_counting = true;
		sleep_ms(1000);		
	}

	return 0;
}

