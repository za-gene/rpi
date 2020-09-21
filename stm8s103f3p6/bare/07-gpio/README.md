# stm8 millis

Toggles the internal LED every 500ms based on millis() count. Note that the millis() 
resets approx every 49 days (=(2^32-1)/1000/60/60/24 = 49.71).

Implementation of millis is in the library in the parent directory. See millis.c.

I'm a little worried that the presence of the line in `stm8.h`:
```
void timer4_millis_isr() __interrupt(TIM4_ISR);
```
will cause confusion for the compiler if I use another TIM4_ISR. It's something
to bear in mind. However, its presence does seem to be required in the
way that sdcc seems to compile and link things. It seems to require "some accommodation"
on the developer's part.
