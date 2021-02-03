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

void set_external_interrupt(PORT_t* port, u8 mode)
{

	disable_interrupts();
#if 0
	if(port> PORTE) goto finis;

	u8* reg;
	u8 offset;

	if(port == PORTE) {
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
#endif
EXTI_CR1 = 0b10000000;
EXTI_CR2 = 0b0;
//	digitalWrite(OUT, REG(0x50A1) & 0b10);
finis:
	enable_interrupts();
}


void my_portd_falling_isr() __interrupt(PORTD_ISR)
{
	digitalToggle(OUT);
	//digitalWrite(OUT, LOW);
}


void main()
{
	//pinMode(IN, INPUT_PULLUP);
	pinMode(IN, INPUT);
	pinMode(OUT, OUTPUT);

	set_external_interrupt(PORTD, FALLING);

	while(1) wfi();
	//	digitalWrite(OUT, digitalRead(IN));
       	//digitalToggle(OUT); wfi(); // round and around, waiting for interrupts
}


