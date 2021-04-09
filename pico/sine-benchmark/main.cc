#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

extern int16_t sin_table[64];


uint64_t from;

void start() { from = time_us_64(); }

void elapsed()
{
	uint64_t now = time_us_64();
	printf("Elapsed %" PRIu64 " us\n", now - from);
}

int main() 
{
	stdio_init_all();
	getchar();

	puts("\nUsing sine table");
	start();
	for(int i = 0; i<64; i++) {
		volatile int64_t x = sin_table[i];
	}
	elapsed(); // 12us, or 0.1875us per calc

	puts("\nUsing sin() function");
	start();
        for(int i = 0; i<64; i++) {
		volatile float f = sin(i);
	};
	elapsed(); // 1013us, or 15.8us per calc (expensive!)

	puts("\n100 x 100 using floats");
	volatile float f1 = 00.0, f2 = 100.0;
	start();
	for(int i = 0; i< 1000; i++) {
		volatile float f = f1 * f2;
	}
	elapsed();  // 737us, or 0.74us per calc

	puts("\n100 x 100 using fixed point");
	volatile int32_t i1 = 100 << 7, i2 = 100 << 7, res1;
	start();
        for(int i = 0; i< 1000; i++) {
		res1 = (i1 * i2) >> 7;
	}
	elapsed(); // 99us, or 0.099us per calc
	printf("... expecting 10000: %d\n", res1 >> 7);


	assert(false); // doesn't actually assert

	puts("\nFinished tests. Halting");
	for(;;);
       
	return 0;
}

