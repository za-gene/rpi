#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void init_timer1(unsigned long freq)
{
        TCCR1 = 0; // stop the counter
        TCNT1 = 0;
        OCR1C = 0;
        if(freq==0) return;

        int prescale = 1;
        while(freq * 256 * (1<<(prescale-1)) < F_CPU)
                prescale++;
        TCCR1 = (1<<CTC1);
	TCCR1 |= prescale;
        int divisor = F_CPU/(1<<(prescale-1))/freq;
        OCR1C = divisor; // compare match value
	TIMSK |= (1<<OCIE1A); // enable compare match interrupt
	sei();
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1<<PB1); // toggle
}

int main()
{
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	init_timer1(5000);

	for(;;); // forever
}
