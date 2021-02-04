# Interrupts

A complete list of interrupts, taken from file `iotnx5.h` :

```
INT0_vect
PCINT0_vect
TIM1_COMPA_vect
TIM1_OVF_vect
TIM0_OVF_vect
EE_RDY_vect // EEPROM ready
ANA_COMP_vect // analog comparator
ADC_vect // ADC conversion ready
TIM1_COMPB_vect
TIM0_COMPA_vect
TIM0_COMPB_vect
WDT_vect
USI_START_vect
USI_OVF_vect
```

`TIM` can be replaced by `TIMER`, e.g. `TIM1_COMPA_vect` as
`TIMER1_COMPA_vect`. 


## Pin change interrupts

You've two options:
* use INT0 for PB2, which can be configured for low, changing, falling or rising levels
* use PCINT0, which can be configured for any pin(s), but which only detect changes. 

## INT0

Untested code:

```
#include "avr/interrupt.h"
 
volatile int value=0;
 
void setup()
{
    GIMSK |= (1<<6);    // turn on pin interrupt for PB2 (it's the only on)
    MCUCR |= 0b10 ; // configure for falling edge
    sei();                 // enables interrupts
}
 
void loop()
{
}
 
ISR(INT0_vect)
{
    value = 1;             // Increment volatile variable
}
```


## PCINT0

Untested code:

```
#include "avr/interrupt.h"
 
volatile int value=0;
 
void setup()
{
    GIMSK |= (1<<5);    // turns on pin change interrupts // PCIE bit
    PCMSK = 0b00010011;    // turn on interrupts on pins PB0, PB1, &amp;amp; PB4
    sei();                 // enables interrupts
}
 
void loop()
{
}
 
ISR(PCINT0_vect)
{
    value = 1;             // Increment volatile variable
}
```

Example: [int0-cli](int0-cli)

## Also in this directory

* [timer1](timer1) - use of Timer1 to fire an interrupt at any frequency
