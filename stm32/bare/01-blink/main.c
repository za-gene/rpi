#include <gpio.h>


void main(void)
{
	// Let's configure some pins for output
	gpio_mode_out(BUILTIN_LED);
	gpio_mode_out(PA4);
	gpio_mode_out(PA5);
	gpio_mode_out(PA7);
	gpio_mode_out(PB1);
	gpio_mode_out(PB13);
	gpio_mode_out(PC15);

	gpio_write(PA7, 1); // set it high permanently

	// let's get some blinky lights going
	// Attach some LEDs if you want to see them in action
	int i;
	while(1)
	{
		gpio_toggle(PA4);
		gpio_toggle(PB1);
		gpio_toggle(PC15);
		gpio_write(BUILTIN_LED, 1);

		gpio_write(PB13, 1);
		for (i = 0; i < 500000; i++); // arbitrary delay
		gpio_write(BUILTIN_LED, 0);
		gpio_write(PB13, 0);
		gpio_toggle(PB1);
		for (i = 0; i < 500000; i++); // arbitrary delay
	}
}


