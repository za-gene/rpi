#include "../blue.h"

// http://embedded-lab.com/blog/stm32-gpio-ports-insights/

/* A more refactored style for the registers can be found at
 * https://github.com/WRansohoff/STM32x0_timer_example/blob/master/device_headers/stm32f031x6.h
 */

// register address
#define RCC_BASE      0x40021000


#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)


// bit fields
#define RCC_IOPBEN   (1<<3)
#define RCC_IOPCEN   (1<<4)
#define GPIO1       (1UL<<1)
#define GPIO13      (1UL<<13)

//static int j = 666; // this seems to cause problems

// pins are defined as hex 0xPQ where P is port, and Q is pin number


#define PA13 0x0D
#define PB1  0x11
#define PB13 0x1D
#define PC13 0x2D

void gpio_mode_out(u32 pin)
{
	u32 port = pin >>4; // gives 0 for Port A, 1 for port B, 2 for port C
	RCC_APB2ENR |= (1 << (port+2)); // enable port
	//GPIO_t* port_x = ((GPIO_t*) (GPIO_BASE + port*0x400));
	u32 GPIOx_CRx = GPIO_BASE+port*0x400; // assume GPIOx_CRL
	u32 pin_num = pin & 0x0F;
	if(pin_num > 7) {
		GPIOx_CRx += 0x04; // bump to GPIOx_CRH;
		pin_num -= 8;
	}
	//GPIOx_CRx =  GPIO_BASE + 0x800 + 0x04;
	u32 mask = 0b1111 << (pin_num*4);
	*(volatile u32*) GPIOx_CRx &= ~mask; // mask out the mode and CNF
	*(volatile u32*) GPIOx_CRx |= (0b0010<< (pin_num*4)) ; //  CNF output push-pull, max speed 2MHz
	//GPIOC->CRH   |= 0x00200000;

}


void main(void)
{
	gpio_mode_out(PC13);

	//RCC_APB2ENR |= RCC_IOPCEN; // enable port C
	//GPIOC->CRH   &= 0xFF0FFFFF;
	//GPIOC->CRH   |= 0x00200000;

	// Set PB13 into a high output state
	RCC_APB2ENR |= RCC_IOPBEN; // enable port B
	// set PB13 for output
	GPIOB->CRH   &= 0xFF0FFFFF;
	GPIOB->CRH   |= 0x00200000;
	// set pin PB13 high
	GPIOB->ODR |= GPIO13;

	// set PB1 for output and set it high
	GPIOB->CRL  &= 0xFFFFFF0F;
	GPIOB->CRL  |= 0x00000020;
	GPIOB->ODR  |= GPIO1;


	int i;
	while(1)
	{
		//GPIOC_ODR |=  GPIO13;
		GPIOC->ODR |= GPIO13;
		for (i = 0; i < 500000; i++); // arbitrary delay
		GPIOC->ODR &= ~GPIO13;
		for (i = 0; i < 500000; i++); // arbitrary delay
	}
}


