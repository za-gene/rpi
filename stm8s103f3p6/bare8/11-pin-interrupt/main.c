#include <stm8.h>
#include <gpio.h>

#define IN PD3
#define OUT PC7

#define FALLING		0b10
#define RISING 		0b01
#define CHANGING	0b11

#define EXTI_CR1 REG(0x50A0)
#define EXTI_CR2 REG(0x50A1)

void put8(u8* addr, u8 value)
{
	*addr = value;
}

u8 get8(u8* addr)
{
	return *addr;
}

/** mode should be one of FALLING, RISING or CHANGING
*/ 

void enable_pin_irq(u8 pin, u8 mode)
{

	disable_interrupts();
	u8 port = pin >> 4;
	if(port>4) goto finis; // must be in ports A..E

	u8* reg;
	u8 offset;

	if(port == 4) { //PORT E
		reg =  (u8*) 0x50A1; // EXTI_CR2;
		offset = 0;
	} else { //PORTS A..D
		reg =  (u8*) 0x50A0; // EXTI_CR1;
		offset = port*2; 
	}

	u8 mask = 0b11 << offset;
	u8 val = get8(reg);
	val &= ~mask;
	val |= (mode << offset);
	put8(reg, val);

	// activate the interrupt for the pin
	PORT_t* p = gpio_base(pin);
	u8 bit = pin & 0xF;
	p->CR2 |= (1<<bit);
finis:
	enable_interrupts();
}


void my_portd_falling_isr() __interrupt(PORTD_ISR)
{
	digitalToggle(OUT);
}


void main()
{
	pinMode(IN, INPUT_PULLUP);
	pinMode(OUT, OUTPUT);
	enable_pin_irq(IN, FALLING);

	while(1);
}


