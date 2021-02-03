#include <stm8.h>
#include <gpio.h>

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

void set_external_interrupt(PORT_t* port, u8 mode)
{

	disable_interrupts();
	if(port> PORTD) goto finis;

	u8* reg;
	u8 offset;

	if(port == PORTD) {
		reg =  (u8*) 0x50A1; // EXTI_CR2;
		offset = 0;
	} else {
		reg =  (u8*) 0x50A0; // EXTI_CR1;
		offset = (port-PORTA)/5*2; // sizeof(PORT_t) s/b 5
	}

	u8 mask = 0b11 << offset;
	u8 val = get8(reg);
	val &= ~mask;
	val |= (mode << offset);
	put8(reg, val);
finis:
	enable_interrupts();
}

#define IN PD3
#define OUT PC7

void my_portd_falling_isr() __interrupt(EXTI3_ISR)
{
	digitalToggle(OUT);
}


void main()
{
	pinMode(IN, INPUT_PULLUP);
	pinMode(OUT, OUTPUT);

	set_external_interrupt(PORTD, FALLING);
	enable_interrupts();

	while(1) wfi(); // round and around, waiting for interrupts
}

