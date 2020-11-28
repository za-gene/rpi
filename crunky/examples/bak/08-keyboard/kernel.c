#include <lfb.h>

#include "rpi-usb.h"


void say(char *str)
{
	fbputs(str);
}

void kernel_main()
{
	lfb_init();
	say("keyobard test started");
	say("Let's try");

	UsbInitialise();
	/* Detect the first keyboard on USB bus */
	uint8_t firstKbd = 0;
	for (int i = 1; i <= MaximumDevices; i++) {
		if (IsKeyboard(i)) {
			firstKbd = i;
			break;
		}
	}
	if (firstKbd) say("Keyboard detected\r\n");

//finis:
	while(1);
}
