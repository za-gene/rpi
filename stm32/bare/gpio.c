#include "gpio.h"

#if 0
void put32(u32 addr, u32 val)
{
	*(volatile u32*) addr = val;
}

u32 get32(u32 addr)
{
	return *(volatile u32*) addr;
}
#endif


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
void gpio_mode_out(u32 pin)
{
	u32 port = pin >>4; // gives 0 for Port A, 1 for port B, 2 for port C

	// enable the port only if it not enabled
	//u32 port_enabled = RCC_APB2ENR & (1 << (port+2));
	//if(port_enabled == 0) 
	RCC_APB2ENR |= (1 << (port+2)); // enable port

	u32 GPIOx_CRx = pin_to_gpio(pin) + 0x00; // assume GPIOx_CRL
	u32 pin_num = pin & 0x0F;
	if(pin_num > 7) {
		GPIOx_CRx += 0x04; // bump to GPIOx_CRH;
		pin_num -= 8;
	}
	u32 mask = 0b1111 << (pin_num*4);
	*(volatile u32*) GPIOx_CRx &= ~mask; // mask out the mode and CNF
	*(volatile u32*) GPIOx_CRx |= (0b0010<< (pin_num*4)) ; //  CNF output push-pull, max speed 2MHz
	//GPIOC->CRH   |= 0x00200000;
	//gpio_write(pin, 0);

}


void gpio_toggle(u32 pin)
{
	u32 GPIO_ODR = pin_to_gpio(pin) + 0x0C;
	u32 on = get32(GPIO_ODR) &  (1<<(pin & 0xF));
	if(on)
		gpio_write(pin, 0);
	else
		gpio_write(pin, 1);
}


