/* 2021-06-06 mcarter started

Connections:
PB12 CS (we manage it ourselves)
PB13 SCLK
PB14 MISO - NOT REQUIRED
PB15 MOSI
MISO not required
*/
#include "../mal.h"

typedef uint8_t u8;
typedef uint32_t u32;

// shut the compiler up about missing protypes
void delay(int n);

void delay(int n)
{
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < 1000; i++)
		{
			__asm__("nop");
		}
	}
}



int main(void)
{
	mal_max7219_init();

	u32 count = 0;
	while (1)
	{
		mal_max7219_show_count(count++);
		delay(4000);
	}
}
