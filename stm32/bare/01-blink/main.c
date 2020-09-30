#include <gpio.h>

#if 0
// http://embedded-lab.com/blog/stm32-gpio-ports-insights/

/* A more refactored style for the registers can be found at
 * https://github.com/WRansohoff/STM32x0_timer_example/blob/master/device_headers/stm32f031x6.h
 */

// register address
#define RCC_BASE      0x40021000


#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)


// bit fields
//#define RCC_IOPBEN   (1<<3)
//#define RCC_IOPCEN   (1<<4)
//#define GPIO1       (1UL<<1)
//#define GPIO13      (1UL<<13)

//static int j = 666; // this seems to cause problems

// pins are defined as hex 0xPQ where P is port, and Q is pin number


#define PA4  0x04
#define PA5  0x05
#define PA7  0x07
#define PA13 0x0D
#define PB0  0x10
#define PB1  0x11
#define PB13 0x1D
#define PC13 0x2D
#define PC15 0x2F

#define BUILTIN_LED PC13

void put32(u32 addr, u32 val)
{
	*(volatile u32*) addr = val;
}

u32 get32(u32 addr)
{
	return *(volatile u32*) addr;
}

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

#endif

void main(void)
{
	//
	//gpio_write(PC13, 0);

	//RCC_APB2ENR |= RCC_IOPCEN; // enable port C
	//GPIOC->CRH   &= 0xFF0FFFFF;
	//GPIOC->CRH   |= 0x00200000;

	// Set PB13 into a high output state
	//RCC_APB2ENR |= RCC_IOPBEN; // enable port B
	// set PB13 for output
	//GPIOB->CRH   &= 0xFF0FFFFF;
	//GPIOB->CRH   |= 0x00200000;
	// set pin PB13 high
	//GPIOB->ODR |= GPIO13;
	gpio_mode_out(PB13);
	//gpio_write(PB13, 1);
	//gpio_write(PB13, 0);

	// set PB1 for output and set it high
	//GPIOB->CRL  &= 0xFFFFFF0F;
	//GPIOB->CRL  |= 0x00000020;
	//GPIOB->ODR  |= GPIO1;
	gpio_mode_out(PB1);

	gpio_mode_out(PC15);
	//gpio_write(PC15, 1);
	gpio_mode_out(BUILTIN_LED);
	gpio_write(BUILTIN_LED, 1); // builtin LED works the opposite way you expect


	int i;
	while(1)
	{
		//GPIOC_ODR |=  GPIO13;
		//GPIOC->ODR |= GPIO13;
		//gpio_write(BUILTIN_LED, 1);
		//gpio_write(PB1, 1);
		gpio_toggle(PB1);
		gpio_toggle(PC15);
		gpio_write(BUILTIN_LED, 1);

		gpio_write(PB13, 1);
		for (i = 0; i < 500000; i++); // arbitrary delay
		//GPIOC->ODR &= ~GPIO13;
		//GPIOC->ODR ^= (1<<13);
		//GPIOC->ODR = 0;
		gpio_write(BUILTIN_LED, 0);
		gpio_write(PB13, 0);
		gpio_toggle(PB1);
		for (i = 0; i < 500000; i++); // arbitrary delay
	}
}


