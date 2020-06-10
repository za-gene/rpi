# timer

Produces a tone of 440Hz using timer2 CTC, no PWM. It shows generalised calculation for the prescaler and OCR register to produce any frequnecy.

**Timers and uses:**
```
       bits purpose
timer0    8 delay(), millis(), micros(), ...
timer1   16 servos
timer2    8 tone()
```

**Minimum frequency**

timer0 and timer2 have a minumum freqency of 61.3Hz, calculated by
`f = 16e6/255/1024`, where 16e6 (i.e. 16MHz) is the clock frequency of the Arduino, 255 is the maximum value of the timer (i.e. 8 bits), and 1024 is the maximum prescaler for timer2. If you want lower frequencies, then using timer1 might be a better bet.

**Normal and CTC mode**

Each timer can have a normal mode, and a CTC (Counter Control Register). AFIAK, in normal mode, interrupts are triggered when the counter overflows. In CTC mode, an additonal counter is available, so that you can achieve the frequencies you want (subject to  certain granularities), rather than using code to achieve a similar effect.

I think of CTC as having an inner and outer loop.

**Prescalers**

It is possible to scale the clock so that the timer2 value (TCNT2) is incremented as a fraction of the clock frequency.

Each timer has different prescalers available to it. For TIMER2, they are:
```
CS22 CS21 CS20    Scaling
   0    0    0    Timer/counter stopped
   0    0    1    1/1 (i.e. no prescaling)
   0    1    0    1/8
   0    1    1    1/32
   1    0    0    1/64
   1    0    1    1/128
   1    1    0    1/256
   1    1    1    1/1024
```

CS22:20 are bits within the TCCR2 (Timer/Counter Control Register).

Given a frequency `f`, calculate a raw prescaler `p' = 16e6/256/f` (where 16e6 is the 16MHz clock speed, and 256 is the maximum value of the counter). Then select `p''`, being the minimum prescaler for which `p' <= p''`

For example, to generate a frequency f= 16kHz, `p'=16e6/256/16e3=3.9`. Therefore choose the scaling of 1/8, because `1 < 3.9 < 8`.

The timer count, `R`, is given by
```
R= 16e6/p''/f-1
```

So, in the example above, `R = 16e6/8/16000 -1 = 124`.

As another example, to generate a freqency of 440Hz:
```
p' = 16e6/256/440 = 142.0
Thus p'' = 256 (128 < 142 < 256)
So R = 16e6/p''/440 -1 = 16e6/256/440-1 = 141
```
