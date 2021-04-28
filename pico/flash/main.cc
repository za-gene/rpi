#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
#include "hardware/sync.h"
#include "tusb.h" // if you want to use tud_cdc_connected()


// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (256 * 1024)

//const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

//#define ADDRESS (XIP_BASE+ 0x5'0000) // 0x9F000)
#define ADDRESS (XIP_BASE+ FLASH_TARGET_OFFSET)
#define SIZE    4096

#define MAGIC 0x1337

// the data we wish to store between power cycles
struct __attribute__((__packed__)) data_t {
	//char magic[4]{ 'D', 'A', 'T', '1'};
	int magic = MAGIC;
	int count = 0;
	char padding[SIZE - sizeof(int) - sizeof(int)]; // we don't actually do anything with this
} data;

//static_assert(sizeof(data) <= FLASH_PAGE_SIZE);
static_assert(sizeof(data) == SIZE);


void write_flash_data()
{
	//puts("Performing memcpy");
	//memcpy((void*) flash_target_contents, &data, sizeof(data));
	puts("Doing flash_range_program");
	uint32_t ints = save_and_disable_interrupts();
	//flash_range_program(FLASH_TARGET_OFFSET, flash_target_contents, FLASH_PAGE_SIZE);
	flash_range_program(FLASH_TARGET_OFFSET, (const uint8_t*) &data, FLASH_PAGE_SIZE);
	restore_interrupts (ints);
	puts("Should be flashed now");
}

int main() 
{
	stdio_init_all();
	while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	//if(strncmp("DATA", (char*) flash_target_contents, 4) == 0) {
	//if(strncmp("DAT1", (char*) (ADDRESS), 4) == 0) {
	if( ((data_t*) (ADDRESS)) ->magic == MAGIC) {		
		puts("Flash has already been set up");
		memcpy(&data, (void*) (ADDRESS), SIZE);
		//data.count += 1;
	} else {
		puts("Flash is uninitialised");
		//write_flash_data();
	}

	//struct data_t 
	//memcpy(&data, (void*) flash_target_contents, sizeof(data));
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

