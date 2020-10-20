# Approx. 440Hz triangular wave using PWM

Attach a speaker with one terminal to PA6, and one to ground. 
This project builds on 
[project 11](../11-pwm).

As we saw om project 11, it is possible to
produce a PWM signal of a given frequency and duty cycle.
This project takes things a step further: varying the duty cycle
over each cycle. It is therefore possible to produce DAC-like
behaviour using PWM. 
I am excited by this prospect, as it means (potentially) that we 
can ditch DACs and just use PWMs.

The ingredients added to this project are:
* interrupts, where we increment the timer's CCR1 (capture/compare register 1)
on each interrupt
* use "preloading" of CCR1 (aka "buffering" or using the "shadow register") so
as to ensure that CCR1 is only updated at the beginning of an ARR (Auto Reload Reset)
rather than at some arbitrary time

Near the top of main.c, we have:
```
#define FREQ 440
#define NSAMPLES 16
```

indicating that we want to generate a signal of 440Hz, and discretise the output "volumes" 
over 16 samples. Our timer will therefore need to be of a frequency of 7040Hz (=440*16),
which is 16X faster than in project 11. 

In `setup_timer()` we have:
```
TIM3->ARR = NSAMPLES -1;
```
for convenience sakes, and we're going to get `TIM3->CCR1` incrementally on each interrupt.

Continuing:
```
TIM3->PSC = 8000000UL/FREQ/NSAMPLES/NSAMPLES ;
```
We're using an 8MHz clock. [Recall](../pwm.md) that 
```
PSC+1 = f\_clk/f\_pwm/NSAMPLES
```
where
```
f\_pwm = FREQ * NSAMPLES
```
yielding the code above.

Further down, we dispense with setting `TIM3->CCR1`, because we are going to do that in the interrupt.
We do need to add the line:
```
TIM3->CCMR1 |= TIM_CCMR1_OC1PE; // enable preload
```
which will allow us to properly sync our new value of CCR1 with the ARR reset.

We initialise the interrupt:
```
NVIC_ISER0 |= (1<<29); // position for TIM3_IRQHandler
TIM3->DIER |= TIM_DIER_UIE; // enable interrupt (this wasn't in project 11)
enable_irq();
```

Here is the interrupt:
```
void TIM3_IRQHandler()
{
	// increment the PWM counter for when to set low
	static u32 count=0;
	if(count >= NSAMPLES) count = 0;
	TIM3->CCR1 = count++;

	TIM3->SR &= ~TIM_SR1_UIF; // reset interrupt flag
}
```

Note that TIM3 is used by pin PA6 on channel 1 for PWM. We could, theoretically, use
a different timer to change CCR1, but there is not point.

`TIM3_IRQHandler()` triggers with a frequency 7040Hz, and we increment CCR1 by 1 each time, 
cycling when we get to a count of NSAMPLES. Note that this produces a triangular wave rather
than a sawtooth one. A pin's maximum volume occurs when the duty cycle is 50%, so as we
increase the duty cycle beyojnd 50%, the volume actually goes down again.

Note that the final line of the interrupt handler reads
```
TIM3->SR &= ~TIM_SR1_UIF; // reset interrupt flag
```
which resets the interrupt so that it doesn't end up being a one-shot.


Works 2020-10-20

