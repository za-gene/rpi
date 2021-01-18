#define F_CPU 8000000UL


//#include <avr/io.h>
//#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//#include <avr/timer.h>

//#include <timer1.h>
#include <uart1.h>

#define SPK (1<<PB1)

typedef uint8_t u8;


#if 0
u8 rnd()
{
	static uint16_t j = 1, i = 0;
	j = (7 * j + i++ ) % 101;
	return j & 1;
}

volatile long r;

ISR(TIMER1_COMPA_vect)
{
	if(random() & 1)
		PORTB |= SPK;
	else
		PORTB &= ~SPK;

	r >>= 1;
}



#endif


unsigned long millis = 0;
ISR(TIMER0_OVF_vect) {
        // this interrupt will happen every 1 ms
        millis++;
        // Clear timer0 counter
        TCNT0 = 130;
    }


void output(unsigned long v)
{
	char str[10];
	utoa(v, str, 10);
	puts_uart1(str);
}

int main()
{
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	// timer 0 setup
	TCNT0 = 130;
	TIMSK = (1<<1);
	TCCR0B = (1<<CS00)|(1<<CS01);
	sei();

	//DDRB = SIPK; 
	//PORTB = SPK;
	//init_timer1(F_CPU, 3000); 

	init_uart1(F_CPU, PB1);
	//_delay_ms(500);
	puts_uart1("---");
	output(CLKPR);
	output(sizeof(unsigned long));
	long start = millis;
	for(unsigned long i = 0; i< 100000UL; i++) {
		volatile long j = random();
	}
	output(millis-start);

	for(;;);

	for(;;) {
#if 0
		long r1 = random();
		if(r1<0) r1 = -r1;
		r = r1;
#endif
	}
}
