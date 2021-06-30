# irq-interrupt

Blink a light using timer (TIM2) interrupts. Took me long enough to get it working!

It blinks on every second. Note that the interrupt toggles the pin, so the frequency
of the irq is set to 2Hz, rather than 1Hz.

One of my problems was that I had to
```
timer_clear_flag(TIM2, TIM_SR_UIF);
```
at the beginning of `void tim2_isr(void)`, rather than at the end.


## NEWS

2021-06-30 Started. Some aspects working (zeroseg).
