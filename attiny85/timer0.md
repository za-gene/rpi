# Timer0

Using avrlib

Setting the clock rate to 8MHz and measure millis as follows:

```
#define F_CPU 8000000UL

unsigned long millis = 0;
ISR(TIMER0_OVF_vect) {
        // this interrupt will happen every 1 ms
        millis++;
        // Clear timer0 counter
        TCNT0 = 130;
}

int main()
{
	// set clock to 8MHz
        CLKPR = (1<<CLKPCE);
        CLKPR = 0;

        // timer 0 setup
        TCNT0 = 130;
        TIMSK = (1<<1);
        TCCR0B = (1<<CS00)|(1<<CS01);
        sei();

	...
}

```


## See also

* [random-benchmark](random-benchmark) - makes use of this

## Links to other sites

* [source](https://stackoverflow.com/questions/62620994/attiny85-microsecond-timer-implemented-on-timer0-does-not-count-the-correct-tim) - inspiration for this code
