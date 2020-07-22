## ADC using interrupts

setSampleRate conversion speeds

```
DEF   X100  EACH  kHz
239_5 2102  21     47
71_5   702   7    142
55_5   569   5.7  175
```

Legend:
* DEF: the parameter passed into setSampleRate(). E.g. 239_5 means ADC_SMPR_239_5
* X100: time taken (microseconds) to perform 100 reads
* EACH: equivalent time per read (us)
* kHz: equivalent frequency (kHz) for each read

Although it might seem logical to max out the sampling frequency, I think this uses the bus a lot, so you might have difficulty getting other work done
