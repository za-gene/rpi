# pwm and timing

PWM on pins are tied to a timer and channel. Refer to the datasheet, section
*9.3.7 Timer alternative function mapping*. For example, pin PA6 is attacked to 
TIM3\_CH1.

```
f\_pwm = f\_clk /(ARR+1)/(PSC+1)

DutyCycle\_pwm (%) = CRRx / ARRx (%)
```

f\_pwm is the frequency of PWM produced. ARR is Auto reload reqgister value, PSC is prescaler,

f\_clk is the frequency of the internal clock, which defaults to 8MHz (not 72MHz, which is the speed of the processor).


## Links to other sites

* [Example](https://deepbluembedded.com/stm32-pwm-example-timer-pwm-mode-tutorial/) containing 
calculations above, and more
