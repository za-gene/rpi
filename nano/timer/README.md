# timer

Produces a tone of 440Hz using timer2 CTC, no PWM. It shows generalised calculation for the prescaler and OCR register to produce any frequency.

This project is useful for learning how to set timers to yield a required frequency. Ultimately, I intend to use it together with a DOC and an SD card to create a media player.

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

## The code

The complete [code](timer.ino) is available as one file, but I will break it down here. All of the above logic can be encapsulated in the following function:

```
void  init_timer2(unsigned int f)
{
  static const unsigned int scales[] = {1, 8, 32, 64, 128, 256, 1024};

  cli();//stop interrupts
  TCCR2A = 0;
  TCNT2  = 0;
  TCCR2A |= (1 << WGM21); // turn on CTC mode
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt

  // calculate prescaler and Match Register Comparison
  unsigned int p;
  for (p = 0; p < 8; p++) if (scales[p] >= 62500 / f) break;
  OCR2A = (62500 / scales[p]) * 256 / f; // Set Compare Match Register
  TCCR2B = ++p; // the prescaler

  sei();//allow interrupts
}
```

You can find some of the meanings of the registers in the references provided at the end. The scalars from the table above are encapsulated in the line
```
static const unsigned int scales[] = {1, 8, 32, 64, 128, 256, 1024};
```

Note that the scalar table is specific to TIMER2. It is not universal to all the timers.

In the `for()` loop, we calculate which scalar we need to employ, as an index. Note that `62500 = 16e6/256`, as elaborated above. Note that we use the precalculated value because 16e6 is too big for an unsigned int. Only a long would be able to hold it, which we consider as unnecessary memory usage.

When we know which prescalar to use, we can set the value of `OCR2A`, which is the value of `R` in the calculations above, as follows:
```
OCR2A = (62500 / scales[p]) * 256 / f; 
```
Two point to note:

* 62500*256 = 16MHz. We have done the calculations in a seemingly strange way so as to avoid the long vs int size issue
* You will notice that in the original formulation of `R`, 1 was subtracted, but has not been done so here. This is because 
`62500/scales[p]` is usually fractional, and rounded down, thereby obviating the need for the decrement. I don't think pathological cases are a concern.

As a final part of the calculation, we set the prescalar:
```
TCCR2B = ++p;
```

Again, there are subtle points to note:

* CS22:CS20 are conveniently described by a (3-bit) integer which increases as does the index of prescalar, albeit with an "off by 1" adjustment required.
* CS22:C20 occupy the least significant bits of `TCCR2B`, so we can make a direct assignment between it and `p`.

That completes the calculation function. Now let's see how it can be used in "client" code. 

Attach an active speaker (i.e. not a piezoelectric one) to pin D4, for the sake of argument, and it's other end to GND (ground). The code uses a suitable definition:
```
#define SPKR 4
```

Create an interrupt that turns the speaker on and off at the frequency set using init_timer2():
```
int toggle  = 0;
ISR(TIMER2_COMPA_vect) { //toggle speaker on/off
  digitalWrite(SPKR, toggle);
  toggle = 1 - toggle;
}
```

Finally, set up the speaker to output a frequency of 440Hz:
```
void setup() {
  Serial.begin(9600);
  pinMode(SPKR, OUTPUT);
  init_timer2(440 * 2);
}

void loop() {
}
```

Note that the line reads `init_timer2(440 * 2)` rather than `init_timer2(440)`. This is because we have to toggle both on *and* off in the 440Hz frequency.

You can check that the frequencies are correct by comparing them to an online synthesiser (see link below)

## References

* [Arduino Timer Interrupts](https://www.instructables.com/id/Arduino-Timer-Interrupts/) : gives the formulae used above, and some code for setting the registers
* [AVR Timers - TIMER2](http://maxembedded.com/2011/06/avr-timers-timer2/) : excellent overview, including what the registers do
* [Online Tone Generator](http://onlinetonegenerator.com/) : play a note of a given frequency. Use the square wave generator for the best comparison 
* [Reddit](https://www.reddit.com/r/arduino/comments/h0i9va/tutorial_calculating_prescalers_and_match/?ref=share&ref_source=link) discussion

