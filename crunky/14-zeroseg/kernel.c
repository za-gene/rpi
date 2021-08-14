#include <bcm2835.h>
#include <stdio.h>
#include <timers.h>


void transfer_7219(uint8_t address, uint8_t value);
void init_7219();

void kernel_main(void)
{
	puts("\nzeroseg example");
	bcm2835_init();
	bcm2835_spi_begin();
	init_7219();

	int idx =0;
	while (1) {
		int r = idx;
		for (uint8_t i = 0; i < 8; ++i) {
			transfer_7219(i+1, r % 10 );
			r /= 10;
			delay_ms(1);
		}
		delay_s(1);
		idx++;
	}
}
