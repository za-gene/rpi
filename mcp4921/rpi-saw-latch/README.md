# pi c

Produce a sawtooth wave using latches.

## IMPORTANT!!!

The function `bcm2835_gpio_ren()` causes the Pi to freeze.
To fix the issue, add the following line to `/boot/config.txt`:
```
dtoverlay=gpio-no-irq
```

## NEWS

2021-09-08	Working. Except that there seems to be a background
hum to the speakers, as if the speakers aren't properly grounded.
Also, there's am occasional glitch-type thing in the output.
Looking at the oscilloscope, it has a lovely 400Hz sawtooth output.
I think it is a real-time problem.


2021-09-08	Started. Latching not yet implemented. 
I used a delay of 19us, which works out at a framerate of 52.6kHz. 
A theoretical 200Hz wave comes out at 174Hz, likely to be hampered
by using delay loops.
