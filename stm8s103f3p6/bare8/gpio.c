//#include <stm8.h>
#include <gpio.h>

//#define is_set(register_8, bit) ((register_8 & (1<<bit)) !=0)

#define GPIO_BASE 0x5000

PORT_t* gpio_base(u8 pin)
{
	return (PORT_t*)(GPIO_BASE + sizeof(PORT_t)*(pin>>4));
}



void digitalWrite(u8 pin, u8 value)
{
	PORT_t* gpio = gpio_base(pin);
	u8 bit = (pin&0xF);
	//SPLIT(pin);
	if(value)
		set_bit(gpio->ODR, bit);
	else
		clear_bit(gpio->ODR, bit);
}

void digitalToggle(u8 pin)
{
	PORT_t* gpio = gpio_base(pin);
	u8 bit = (pin&0xF);
	//SPLIT(pin);
	//if(digitalRead(pin))
	digitalWrite(pin, 1-digitalRead(pin));
}

u8 digitalRead(u8 pin)
{
	PORT_t* gpio = gpio_base(pin);
	u8 bit = (pin&0xF);
	//SPLIT(pin);
	return is_set(gpio->IDR, bit);
}

void pinMode(u8 pin, u8 mode)
{
	PORT_t* gpio = gpio_base(pin);
	u8 bit = (pin&0xF);
	//SPLIT(pin);
	switch(mode) {
		case INPUT:
			clear_bit(gpio->DDR, bit);
			clear_bit(gpio->CR1, bit);
			break;
		case INPUT_PULLUP:
			clear_bit(gpio->DDR, bit);
			set_bit(gpio->CR1, bit);
			break;
		case OUTPUT:
			set_bit(gpio->DDR, bit);
			set_bit(gpio->CR1, bit);
			break;
	}
}
