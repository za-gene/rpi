# hertz

Measures frequencies. It's more convenient than an oscilloscope

* (hertz-pico](hertz-pico)
* [nano-arduino](nano-arduino) - flaky above 70kHz 
(NB: that's probably because I use uint16_t count). 
Connect input to pin D3 (3V3 is OK, as it's purely an input).
Output goes to serial (115200 baud). 
Requires 
[MsTimer2](https://github.com/PaulStoffregen/MsTimer2)
