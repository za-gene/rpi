#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include <math.h>
//#include <tusb.h> // if you want to use tud_cdc_connected()


#include "crc8.h"
#include "fat32.h"
#include "pi.h"
#include "ssd1306.h"

#define ALARM 0
#define DELAY (200'000)

//#define ADDRESS (XIP_BASE+ FLASH_TARGET_OFFSET)
#define FLASH_TARGET_OFFSET (256 * 1024)
#define ADDRESS (XIP_BASE+ FLASH_TARGET_OFFSET)
static_assert(FLASH_PAGE_SIZE == 256);

/* acknowledge that transmitter can proceed
*/
void ack(void)
{
	putchar('A');
}

uint32_t size = 0;


void incoming(void)
{
	ssd1306_print("RX...\n");
	uart_read_blocking(uart0, (uint8_t*) &size, 4);
	//size = __builtin_bswap32(size);
	//char msg[80];
	//sprintf(msg, "size %d\n", size);
	//ssd1306_print(msg);
	ssd1306_printf("size is %d\n", size);

	ack();

	// read chunks of data
	//static_assert(FLASH_PAGE_SIZE==256);
	uint8_t sector[4096];
	int unread = size;
	uint32_t offset = FLASH_TARGET_OFFSET;
	int sector_num = 0;
	while(unread >0) {
		ssd1306_print("uart rec ... ");
		uart_read_blocking(uart0, sector, 4096);
		ssd1306_puts("OK");

		uint32_t ints = save_and_disable_interrupts();
		flash_range_erase(offset, 4096);
		flash_range_program(offset, sector, 4096);
		restore_interrupts(ints);
		ssd1306_printf("Sector in: %d\n", sector_num++);
		
		unread -= 4096;
		offset += 4096;
		ack();
	}

	ssd1306_print("OK\n");
	//show_scr();
}

void outgoing(void)
{
	//printf("%"
	ssd1306_print("Sending contents\n");
	//show_scr();
	uart_write_blocking(uart0, (const uint8_t*) &size, 4);

	//return;
	//uint32_t page = 0;
	//uint8_t data[4096];
	for(int i = 0 ; i < size; i++) {
		uart_write_blocking(uart0, (const uint8_t*)(ADDRESS+i), 1);
		//uart_write_blocking(uart0, ADDRESS+i, 1);
		//putchar(buf[i]);
	}
	ssd1306_print("Done\n");
	//show_scr();
	//write(fd1, &cmd, 1);
}

static void alarm_0_irq() 
{
	pi_alarm_rearm(ALARM, DELAY);
	show_scr();
	//printf("Alarm IRQ fired %d\n", i++);
}
int main() 
{
	init_display(64, 8);
	pi_alarm_init(ALARM, alarm_0_irq, DELAY);
	ssd1306_puts("transfer prog");

	stdio_init_all();
	//while(!tud_cdc_connected()) sleep_ms(10); // wait for usb serial 
	ssd1306_puts("Serial up");

#if 0
	printf("ADDRESS = %d\n", ADDRESS);
	printf("XIP_BASE = %d\n", XIP_BASE);
	printf("FLASH_TARGET_OFFSET = %d\n", FLASH_TARGET_OFFSET);
	printf("FLASH_PAGE_SIZE = %d\n", FLASH_PAGE_SIZE); // 256
	printf("FLASH_SECTOR_SIZE = %d\n", FLASH_SECTOR_SIZE); // 4096
#endif
	//printf(" = %d\n", );


#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	//show_scr();
	int i = 0;
	for(;;) {
		char c;
		uart_read_blocking(uart0, (uint8_t*) &c, 1); 
		//char c = getchar();
		switch(c) {
			case 'T' : incoming(); break;
			case 'R' : outgoing(); break;
		}
		/*
		   gpio_put(LED, 1);
		   sleep_ms(100);
		   gpio_put(LED, 0);
		   sleep_ms(1000);	
		   */
	}

	return 0;
}

