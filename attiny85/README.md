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


IN DEPTH:
Pin 1: PB5: PCINT5  ~RESET  ADC0 
Pin 2: PB3: PCINT3  XTAL1   CLKI  ~OC1B   ADC3
Pin 3: PB4: PCINT4  XTAL2   CLKO   OC1B   ADC2
Pin 4: GND
Pin 5: PB0: MOSI DI SDA AIN0 OC0A ~OC1A AREF PCINT0
Pin 6: PB1: MISO DO AIN1 OC0B OC1A PCINT1
Pin 7: PB2: SCK USCK SCL ADC1 T0 INT0 PCINT2
Pin 8: VCC
```

## setup AVR g++

```
sudo apt install gcc-avr avr-libc avrdude
```


## In this directory

* [adc](adc.md) - Adnalog to Digital Conversion
* [Arduino-IDE](Arduino-IDE.md) - setting it up for the ATTiny85
* [digistump](digistump.md)
* [gpio](gpio.md) - low-level notes
* [pwm](pwm.md)
* [tone440](tone440) - produce a 440Hz tone on PB1 (pin 6). See also [audio](../audio)


## Also on this site

* [clock](clock.md) - setting the clock frequency
* [oled85](../1306/oled85) SSD1306
* [t85-spi-sw-master-ide](../attiny85/t85-spi-sw-master-ide) - as a transmitter


## Links to other sites

* [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore)
* [ATtinySerialOut](https://github.com/ArminJo/ATtinySerialOut)
* [Pin changes and timer interrupts](https://embeddedthoughts.com/2016/06/06/attiny85-introduction-to-pin-change-and-timer-interrupts/)
* [megaphone](https://www.youtube.com/watch?v=__ECzxnvEg8)
* [narcoleptic](https://github.com/brabl2/narcoleptic) - 
reduced power consumption
* [oled](https://www.youtube.com/watch?v=_J_96iEm2-g)
* [Software UART with Timer1](https://stackoverflow.com/questions/60997804/attiny85-software-uart-with-timer1)

