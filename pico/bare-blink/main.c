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



void on()
{
	SIO_GPIO_OUT_SET = 1ul << LED; 
}

void off()
{
	SIO_GPIO_OUT_CLR = 1ul << LED; // turn off the LED
}


int main()
{
	// gpio_init(25) equiv
	SIO_GPIO_OE_CLR = 1ul << LED;	// enable output clearing
	SIO_GPIO_OUT_CLR = 1ul << LED;	// now clear it
	// peform the equiv of gpio_set_function()
	// In PADS_BANK0 we'd usually set IE to 1, and OD to 0, but this is the default anyway, so don't bother.
	PADS_BANK0_GPIO25 = 0b1010110;
	IO_BANK0_GPIO25_CTRL = GPIO_FUNC_SIO; // set the pin to act as a regular GPIO pin

	// set direction to output
	SIO_GPIO_OE = 1ul << LED; // not sure if this will be effective. Nope. It wasn't
	SIO_GPIO_OE_SET = 1ul << LED; // enable output setting
	SIO_GPIO_OUT_SET = 1ul << LED; 

	// set pin high
	SIO_GPIO_OUT = 1ul << LED;
	IO_BANK0_GPIO25_CTRL |= (0x3 << 8) | (0x3 << 12); // guess that didn't work

	SIO_GPIO_OUT_SET = 1ul << LED; 

	//while(1);
	SIO_GPIO_OUT_CLR = 1ul << LED; // turn off the LED

	volatile int i = 0;
	while(1) {
		SIO_GPIO_OUT_SET = 1ul << LED; 
	//SIO_GPIO_OUT = 1ul << LED;
		delay(100);
		SIO_GPIO_OUT_CLR = 1ul << LED; // turn off the LED
	//SIO_GPIO_OUT = 0;

		delay(900);
		i++;
	}

	return 0;
}
