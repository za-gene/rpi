#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "hardware/adc.h"
//#include "hardware/clocks.h"
//#include "hardware/flash.h"
#include "hardware/gpio.h"
//#include "hardware/irq.h"
//#include "hardware/pwm.h"
//#include "hardware/spi.h"
// #include "tusb.h" // if you want to use tud_cdc_connected()

#define REG(addr) *(volatile uint32_t*)(addr)


//#define SIO_BASE 		0xd0000000 // see s2.3.1.7
#define SIO_GPIO_OUT		REG(SIO_BASE+0x010) // GPIO output value
#define SIO_GPIO_OUT_SET	REG(SIO_BASE+0x014) // GPIO output value set
#define SIO_GPIO_OUT_CLR	REG(SIO_BASE+0x018) // GPIO output value clear
#define SIO_GPIO_OE		REG(SIO_BASE+0x020) // GPIO output enable
#define SIO_GPIO_OE_SET		REG(SIO_BASE+0x024) // GPIO output enable set
#define SIO_GPIO_OE_CLR 	REG(SIO_BASE+0x028) // GPIO output enable clear

//#define IO_BANK0_BASE 		0x40014000
#define IO_BANK0_GPIO25_CTRL 	REG(IO_BANK0_BASE+0x0cc)

//#define PADS_BANK0_BASE 	0x4001c000 // see s2.19.6.3. Pad control register
#define PADS_BANK0_GPIO25	REG(PADS_BANK0_BASE+0x68)


#define GPIO_FUNC_SIO	5



#define BTN  14 // GPIO number, not physical pin
#define LED  25 // GPIO of built-in LED

int main() 
{
	stdio_init_all();
	// while(!tud_cdc_connected()) sleep_ms(250); // wait for usb serial 

	// dump out the second-stage bootloader
	const int wpl = 4; // words per line
	const int nrows = 256/4/wpl; // number of rows to output
	puts("\n.section .boot2, \"a\"");
	for(int r = 0; r< nrows; r++) {
		printf(".word ");
		for(int c = 0; c<wpl; c++) {
			printf("0x%8.8x", *(uint32_t*) (0x10000000 + (r*wpl+c)*4));
			if(c<(wpl -1)) printf(", ");
		}
		puts("");
	}

	gpio_init(BTN);
	gpio_set_dir(BTN, GPIO_IN);
	gpio_pull_up(BTN);
	// gpio_get() gets state of pin

	gpio_init(LED);
	gpio_set_dir(LED, GPIO_OUT);

	int i = 0;
	for(;;) {
		//printf("Hello number %d\n", i++);
		SIO_GPIO_OUT_SET = 1ul << LED; // turn LED on. Works
		//gpio_put(LED, 1);
		sleep_ms(100);
		//gpio_put(LED, 0);
		SIO_GPIO_OUT_CLR = 1ul << LED; // turn off the LED.Works.
		sleep_ms(1000);		
	}

	return 0;
}

