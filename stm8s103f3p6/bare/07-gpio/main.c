#include <stm8.h>

#define BUTTON PD3

void main()
{
	gpio_mode_out(BUILTIN_LED);
	gpio_mode_pullup(BUTTON);

	while (1)
	{
		u8 high = gpio_read(BUTTON);
		gpio_write(BUILTIN_LED, high);
	}
}

