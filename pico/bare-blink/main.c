#include <stdint.h>

/* The gpio functions are described in the file:
 * src/rp2_common/hardware_gpio/gpio.c
 * Some are inlined in gpio.h (e.g. gpio_set_dir())
 */

#define REG(addr) *(volatile uint32_t*)(addr)


#define SIO_BASE 		0xd0000000 // see s2.3.1.7
#define SIO_GPIO_OUT		REG(SIO_BASE+0x010) // GPIO output value
#define SIO_GPIO_OUT_SET	REG(SIO_BASE+0x014) // GPIO output value set
#define SIO_GPIO_OUT_CLR	REG(SIO_BASE+0x018) // GPIO output value clear
#define SIO_GPIO_OE		REG(SIO_BASE+0x020) // GPIO output enable
#define SIO_GPIO_OE_SET		REG(SIO_BASE+0x024) // GPIO output enable set
#define SIO_GPIO_OE_CLR 	REG(SIO_BASE+0x028) // GPIO output enable clear

#define RESETS_BASE		0x4000c000 // see s2.14.3
#define RESETS_RESET		REG(RESETS_BASE+0x00)
#define RESETS_RESET_DONE	REG(RESETS_BASE+0x08)

#define IO_BANK0_BASE 		0x40014000
#define IO_BANK0_GPIO25_CTRL 	REG(IO_BANK0_BASE+0x0cc)

#define PADS_BANK0_BASE 	0x4001c000 // see s2.19.6.3. Pad control register
#define PADS_BANK0_GPIO25	REG(PADS_BANK0_BASE+0x68)


#define GPIO_FUNC_SIO	5


#define LED 25

void delay(int n) // no particular timing
{
	for(int i =0 ; i< n; i++) {
		for(int j = 0; j< 1000; j++) {
			asm volatile ("nop");
		}
	}
}


// stub for exit
void exit(int status)
{
	while(1);
}

int main()
{
	// inspired by Ada. Appears to be necessary, too.
	RESETS_RESET &= ~(1ul << 5); // clear IO_BANK0
	RESETS_RESET &= ~(1ul << 8); // clear PAD_BANK0
	while(1) {
		int io_bank0_done = (RESETS_RESET_DONE & (1ul<<5)) >0;
		int pad_bank0_done = (RESETS_RESET_DONE & (1ul<<8)) >0;
		if(io_bank0_done && pad_bank0_done) break;
	}
	PADS_BANK0_GPIO25 &= ~(1<<7); // clear output disable 
	PADS_BANK0_GPIO25 &= ~(1<<6); // clear input enable

	//delay(100); // crude method of reset waiting. doesn't seem to help

	// added 2021-06-27
	//volatile uint32_t tmp = PADS_BANK0_GPIO25;
	//PADS_BANK0_GPIO25 = 0b1010110;
	// stuff taken from rust blink example
	//SIO_GPIO_OE_CLR = 1ul<<25; // set gpio25 to be an input (output enable is cleared)
	//SIO_GPIO_OUT_CLR = 1ul<<25; // set gpio25 to be an output low (output is cleared)
	//PADS_BANK0_GPIO25 = 0b1010110 ; //output disable off, input enable on
	
	IO_BANK0_GPIO25_CTRL = GPIO_FUNC_SIO; // init pin - select function SIO
	SIO_GPIO_OE_SET = 1ul << LED; // allow setting of output

	while(1) {
		SIO_GPIO_OUT_SET = 1ul << LED; 
		delay(100);
		SIO_GPIO_OUT_CLR = 1ul << LED; // turn off the LED
		delay(900);
	}

	return 0;
}
