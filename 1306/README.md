# 1306

OLED SSD1306, I2C, 128x64, 12864 LCD screen

Connection:

```
         GND VDD SCK SDA
ATTINY85 GND 3V  PB2 PB0
PICO     GND 3V  GP5 GP4
STM32    GND 3V  PB6 PB7
WROOM    GND 3V  D22 D21
```

**NB** A lot of code out there has addresses of 0x3C for 128x64 OLEDs, and 0x3D for 128x32 OLEDs.
A scan using `i2cdetect -y 1` showed that both types have addresses of 0x3C.

## Pico Micropython

Load ssd1306.py into Pico. Then pico1306.py.

It seems that it can get into a stuck state sometimes, so you
may find it helps to recycle the power.

## Also in this directory

* [fonts](fonts.md) - discussion of `figlet`
* [oled85](oled85) - for the ATTiny85. It works, but the display isn't perfect. [xlink](https://www.instructables.com/ATTiny85-connects-to-I2C-OLED-display-Great-Things/).See also db5/218 for toy module demo.
* [pico-sdk](pico-sdk) - written in C++
* [wroom](wroom.md)

## Links to other sites

* [fonts](https://github.com/lexus2k/ssd1306/blob/master/src/ssd1306_fonts.c) - variety to choose from
* [oscilloscope.py](https://gist.github.com/blippy/dd93dc64640f31dd9616af8e35cc602a) - seems a slow old thing
* [STM8_I2C_OLED](https://github.com/tugaozi/STM8_I2C_OLED) seems "bare metal"

