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

## Also in this directory

* [timer1](timer1) - use of Timer1 to fire an interrupt at any frequency
