# Play sawtooth wave using a DAC (mcp4921) and DMA

**Speed** I used HARE pin to test the speed. Setting the DMA takes about 0.1us - 0.2us (inc pin toggling).
The transfer itself takes about 1us with a SPI baud rate of 17MHz. 
So transfer is quick in any case, although DMA gives us a speed-up for free, at the cost of a
little extra code, of course.


## Status

2021-08-09	Started. Working.

