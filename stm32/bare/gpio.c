#include "gpio.h"

u32 pin_to_gpio(u32 pin)
{
	u32 port = pin>>4;
	return GPIO_BASE + port*0x400;
}

void gpio_write(u32 pin, u32 val)
{
	u32 pos = pin & 0xF;
	u32 GPIO_BSRR = pin_to_gpio(pin) +  0x10;
	if(val == 0 ) pos += 16; // Reset rather than Set
	put32(GPIO_BSRR, 1<<pos);

}

// return either GPIO_CRL, GPIO_CRH, and a pin offset into it
static void gpio_crx_offset(u32 pin, u32 *crx, u32 *offset)
{
	*crx = pin_to_gpio(pin) + 0x00; // assume GPIOx_CRL
	u32 pin_num = pin & 0x0F;
	if(pin_num > 7) {
		*crx += 0x04; // bump to GPIOx_CRH;
		pin_num -= 8;
	}
	*offset = pin_num*4;
}

void gpio_mode(u32 pin, u8 mode)
{
	u32 port = pin >>4; // gives 0 for Port A, 1 for port B, 2 for port C

	// only enable port if currently unenabled
	if((RCC_APB2ENR & (1<<(port+2))) == 0)
		RCC_APB2ENR |= (1 << (port+2)); // enable port

	u32 crx, offset;
	gpio_crx_offset(pin, &crx, &offset);
	u32 mask = 0b1111 << offset;
	*(volatile u32*) crx &= ~mask; // mask out the mode and CNF
	*(volatile u32*) crx |= (((u32)mode)<< offset); 
}

void gpio_mode_out(u32 pin)
{
	gpio_mode(pin, OUTPUT);
}


void gpio_toggle(u32 pin)
{
	gpio_write(pin, 1-gpio_read(pin));
#if 0
	u32 GPIO_ODR = pin_to_gpio(pin) + 0x0C;
	u32 on = get32(GPIO_ODR) &  (1<<(pin & 0xF));
	if(on)
		gpio_write(pin, 0);
	else
		gpio_write(pin, 1);
#endif
}

u8 gpio_read(u32 pin)
{
	u32 crx, offset;
	gpio_crx_offset(pin, &crx, &offset);
	u32 config = get32(crx); // the CNF and MODE settings for the whole port
	u32 mode = config & ( 0b11 << offset); // mask out non-MODE bits

	// determine whether to read IDR or ODR
	u32 addr = pin_to_gpio(pin); // GPIO A/B/C
#if 1
	if(mode) { 
		// it's an output
		addr += 0x0C; // we're now pointing to GPIOx_ODR
	} else {
		addr += 0x08; // we're now pointint to GPIOx_IDR
	}
#else
	addr += 0x0C;
#endif
	u32 port_data = get32(addr);
	if(port_data & (1<< (pin & 0xF)))
		return 1;
	else
		return 0;
#if 0
	u32 GPIO_ODR = pin_to_gpio(pin) + 0x0C;
	u32 on = get32(GPIO_ODR) & (1<<(pin & 0xF));
	if(on) return 1;
	return 0;
#endif
}

