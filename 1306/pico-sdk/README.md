# OLED

Demo code for 128x64 and 128x32 I2C OLED.

**NB** If you specify the height to be 32 on a 64 height display, you
will get double-height output, which could be useful.


## Connections

```
Connect 3V3 from Pico to OLED, and GND to GND, obviously.

SDA is configured for GP4, and SCL on GP5 of Pico.


## News

2021-04-01 Added setCursorx(), setCursory() (https://github.com/blippy/rpi/issues/4). Both 32- and
64- height screens work.

2021-02-11 Added bitmap displays

2021-01-25 Confirmed working
