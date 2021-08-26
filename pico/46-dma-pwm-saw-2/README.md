# Create a saw wave using DMA and PWM

Like project 45, but this time using only 8 bits

## Nuances

if you write to a 32 bit hardware register with an 8 bit value, then thoses 8 bits are duplicated across all 4 bytes for a 32bit write. 
([link)(https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=318404&e=1&view=unread#unread))
It means that I have to pad the 8-bit value to 16 bits, which get duplicated in the level register, but no matter.
I therefore decided that the best approach would therefore to just do single DMA writes of 16 bits.
So in the end it doesn't end up too different from a timer interrupt or PWM wrap interrupt.


## Status

2021-08-26	Working.

2021-08-25	Started. Partially working, but it's very quiet.
