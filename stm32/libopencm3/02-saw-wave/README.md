# saw wave

Uses mcp4921 and timer interrupts. Wave is 440Hz. Output is perfect on a loudspeaker
but rubbish on a small speaker. The problem is likely to be lack of power: as noted
below output was at about 13mA. So power is approx 36mW (2.77V * 13mA). 
So an amplification of 100X would be great.


## Technical Notes

Rigging the DAC to constant max output voltage from STM32 was 2.77V, and 13mA current. 


Running `do_periodic()` outside a timr and insider a tight loop showed that the frequency
of the pin toggling was about 24kHz. This is way short of a desirable 44kHz frame rate. 
The chief culprit seems to be SPI, although floats didn't help. An onboard DAC and 
fixed precision floats would certainly have helped.
However, I decided to use an IRQ at 16kHz. It works


## References

* db05.308


## NEWS

2021-07-01	Works.

2021-06-30 	Started. Some aspects working (zeroseg).
