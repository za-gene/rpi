#include <basal.h>
#include <timers.h>
#include <gpio.h>
#include <interrupts.h>

const int pin = 26;





void blink_led()
{
	gpio_toggle(pin);
	ARM_TIMER_CLI = 0; // clear the timer interrupt
}



void kernel_main ( void )
{
	gpio_sel(pin, OUTPUT);

	u32 freq = 500*2;
	init_arm_timer(freq);

	enable_arm_timer_irq();
	set_irq_handler(blink_led);
	enable_irq();
}
