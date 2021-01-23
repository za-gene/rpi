# 1306

OLED SSD1306, I2C, 128x64, 12864 LCD screen

Connection:

```
         GND VDD SCK SDA
ATTINY85 GND 3V  PB2 PB0
STM32    GND 3V  PB6 PB7
WROOM    GND 3V  D22 D21
```


## Pico Micropython

Load ssd1306.py into Pico. Then pico1306.py.

It seems that it can get into a stuck state sometimes, so you
may find it helps to recycle the power.

## Also in this directory

* [fonts](fonts.md) - discussion of `figlet`
* [oled85](oled85) - for the ATTiny85. It works, but the display isn't perfect. [xlink](https://www.instructables.com/ATTiny85-connects-to-I2C-OLED-display-Great-Things/).See also db5/218 for toy module demo.
* [wroom](wroom.md)

## Links to other sites

* [oscilloscope.py](https://gist.github.com/blippy/dd93dc64640f31dd9616af8e35cc602a) - seems a slow old thing
* [STM8_I2C_OLED](https://github.com/tugaozi/STM8_I2C_OLED) seems "bare metal"

