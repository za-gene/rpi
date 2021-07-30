#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
#include "pi.h"
#include "ssd1306.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#define ALARM 0
#define DELAY (200'000)

char buf[4096];
uint32_t size = 0;

void incoming(void)
{
	char c;
	size = 0;
	ssd1306_print("RX...\n");
	//show_scr();
	for(int i = 0;  i<4; i++) {
		c = getchar();
		size += (c << (i*8));
	}

	
	char msg[80];
	sprintf(msg, "size %d\n", size);
	ssd1306_print(msg);
	//show_scr();
	
	//char c;
	for(int i = 0; i< size; i++) {
		uart_read_blocking(uart0, (uint8_t*) &c, 1);
		buf[i] = c;
		//char c = getchar();
		ssd1306_putchar(c);
		//show_scr();
	}

	ssd1306_print("OK\n");
	//show_scr();
}

void outgoing(void)
{
	//printf("%"
	ssd1306_print("Sending contents\n");
	//show_scr();
#if 1
	uart_write_blocking(uart0, (const uint8_t*) &size, 4);
#else
	uint32_t x = size;
	for(int i = 0; i< 4; i++) {
		//nt32_t y = x >> 24;
		//uart_tx_wait_blocking(uart0);
		putchar(x);
		x >>= 8;
	}
#endif

	//return;
	for(int i = 0 ; i < size; i++) {
		uart_write_blocking(uart0, (const uint8_t*)(buf+i), 1);
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
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED
	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);
	init_display(64, 4);

	pi_alarm_init(ALARM, alarm_0_irq, DELAY);

	ssd1306_print("transfer prog\n");
	//show_scr();
	int i = 0;
	for(;;) {
		char c = getchar();
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

