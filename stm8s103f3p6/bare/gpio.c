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


/*
#define PD4 1
#define PD5 2
#define PD6 3
#define PA1 5
#define PA2 6
#define PA3 10
#define PB5 11
#define PB4 12
#define PC3 13
#define PC4 14 
#define PC5 15
#define PC6 16
#define PC7 17 
#define PD1 18 
#define PD2 19 
#define PD3 20

#define BUILTIN_LED PB5

PORT_t* pin_to_port(u8 pin)
{
	if(pin>=18) return PORTD;
	if(pin>=13) return PORTC;
	if(pin>=11) return PORTB;
	if(pin>=5)  return PORTA;
	return PORTD;
}

u8 pin_to_pos(u8 pin)
{
	if(pin<=3) return pin+3; // PD4..6
	if(pin<=6) return pin-4; // PA1..2
	if(pin==10) return 3; // PA3
	if(pin==11) return 5; // PB5
	if(pin==12) return 4; // PB4
	if(pin<=17) return pin-10; // PC3..7
	return pin-17; // PD1..20
}

void gpio_mode_out(u8 pin)
{
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	port->DDR |= (1<< pos); // direction is output
	port->CR1 |= (1<<pos); // "push-pull"
}

void gpio_mode_in(u8 pin)
{
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	port->DDR &= ~(1<<pos); // input
	port->CR1 &= ~(1<<pos); // floating
}

void gpio_mode_pullup(u8 pin)
{
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	port->DDR &= ~ (1<<pos); // input
	port->CR1 |= (1<<pos); // pullup
}

void gpio_toggle(u8 pin)
{
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	port->ODR ^= (1<<pos);

}

u8 gpio_read(u8 pin)
{
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	return ((port->IDR) & (1<<pos)) ? 1 : 0;
}

void gpio_write(u8 pin, u8 val)
{
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	if(val>0)
		port->ODR |= (1<<pos);
	else
		port->ODR &= ~(1<<pos);

}
*/
