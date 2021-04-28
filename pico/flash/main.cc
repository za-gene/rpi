#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
#include "tusb.h" // if you want to use tud_cdc_connected()


// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (256 * 1024)

const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

// the data we wish to store between power cycles
struct data_t {
	char magic[4]{ 'D', 'A', 'T', 'A'};
	int count = 0;
} data;

//const int num_flash_pages = ceil (sizeof(data)
static_assert(sizeof(data) <= FLASH_PAGE_SIZE);

void write_flash_data()
{
	puts("Performing memcpy");
	memcpy((void*) flash_target_contents, &data, sizeof(data));
	puts("Doing flash_range_program");
	flash_range_program(FLASH_TARGET_OFFSET, flash_target_contents, FLASH_PAGE_SIZE);
	puts("Should be flashed now");
}

int main() 
{
	stdio_init_all();
	while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	if(strncmp("DATA", (char*) flash_target_contents, 4) == 0) {
		puts("Flash has already been set up");
		//data.count += 1;
	} else {
		puts("Initialising flash");
		write_flash_data();
	}

	//struct data_t 
	memcpy(&data, (void*) flash_target_contents, sizeof(data));
	data.count += 1;
	write_flash_data();
	printf("Data count is %d. Bye\n", data.count);




#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		//printf("Hello number %d\n", i++);
		gpio_put(LED, 1);
		sleep_ms(100);
		gpio_put(LED, 0);
		sleep_ms(1000);		
	}

	return 0;
}

