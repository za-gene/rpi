#include <gpio.h>

#define BUTTON PD3

void main()
{
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP);

	while (1)
	{
		u8 high = digitalRead(BUTTON);
		digitalWrite(LED_BUILTIN, high);
	}
}

