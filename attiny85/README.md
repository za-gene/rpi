# ATTiny85

Necessary, but not sufficient:
```sudo apt install avr-libc```

## pins

```
             _________
             |   U   |
Reset (PB5)  | 1   8 |  VCC (+)
(A3)   PB3   | 2   7 |  PB2 (A1, SCK, SCL)
(A2)   PB4   | 3   6 |  PB1 (PWM, MISO)
(-)    GND   | 4   5 |  PB0 (PWM, AREF, MOSI, SDA)
             _________

Notes:
* An means "Analog Input n"
* PB means PORTB
* PB5 requires a "fuse" to be set in order to use it.
```

## setup AVR g++

```
sudo apt install gcc-avr avr-libc avrdude
```


## In this directory

* [Arduino-IDE](Arduino-IDE.md) - setting it up for the ATTiny85
* [gpio][gpio.md] - low-level notes
* [tone440](tone440) - produce a 440Hz tone on PB1 (pin 6). See also [audio](../audio)


## Also on this site

* [clock](clock.md) - setting the clock frequency


## Links to other sites

* [megaphone](https://www.youtube.com/watch?v=__ECzxnvEg8)
* [narcoleptic](https://github.com/brabl2/narcoleptic) - 
reduced power consumption

