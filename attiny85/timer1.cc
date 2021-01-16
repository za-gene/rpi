#include <timer1.h>
//#define F_CPU 1000000UL

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>


//#define PIN (1<<PB2)

/** Initialise Timer1 for an interrupt
 *
 * You must implement ISR(TIM1_COMPA_vect. See project
 * timer1 for an example
 */
void init_timer1(unsigned long f_cpu, unsigned long freq)
{
        TCCR1 = 0; // stop the counter
        TCNT1 = 0;
        OCR1C = 0;
        if(freq==0) return;

        int prescale = 1;
        while(freq * 256 * (1<<(prescale-1)) < f_cpu)
                prescale++;
        TCCR1 = (1<<CTC1);
	TCCR1 |= prescale;
        int divisor = f_cpu/(1<<(prescale-1))/freq;
        OCR1C = divisor; // compare match value
	TIMSK |= (1<<OCIE1A); // enable compare match interrupt
	sei();
}
