#include "pico/stdlib.h"
#include "ssd1306.h"

extern "C" const uint8_t splash1_data[];
int main()
{
	//init_i2c();

	int h = 32;
	if(1) h = 64;
	int sda = 6;
	sda = 4;
	init_display(h, 4);

	ssd1306_print("HELLO PICO...\n"); // demonstrate some text
	show_scr();
	sleep_ms(2000);
	fill_scr(0); // empty the screen

	drawBitmap(0, 0, splash1_data, 64, 64, 1);
	show_scr();

	sleep_ms(2000);
	fill_scr(0);
	setCursorx(0);
	setCursory(0);
	ssd1306_print("Testing cursor");
	show_scr();
	sleep_ms(2000);
	setCursorx(0);
	ssd1306_print("Overwritten   ");
	show_scr();

	for(;;);
	return 0;
}
