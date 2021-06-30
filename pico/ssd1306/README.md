# OLED

Demo code for 128x64 and 128x32 I2C OLED.

**NB** If you specify the height to be 32 on a 64 height display, you
will get double-height output, which could be useful.


## Connections

Connect 3V3 from Pico to OLED, and GND to GND, obviously.

I2C is flexible. Just call `init_display(int h, int sda)` with the relevant SDA pin. The code will
figure out the rest.

In the example provided, SDA is configured for GP6, and SCL for GP7. This is i2c1.

The default config should mean that the Maker Pico will work consistently with the
default CircuitPython setup.




## News

2021-05-23 Added flexibility as to which I2C pins to use

2021-04-01 Added setCursorx(), setCursory() (https://github.com/blippy/rpi/issues/4). Both 32- and
64- height screens work.

2021-02-11 Added bitmap displays

2021-01-25 Confirmed working
