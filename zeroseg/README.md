# zeroseg

SPI

VCC should be connected to 5V, even for 3V3 MCUs


**`basic.py`** is my water-down version of putting a random char (one of: 5, H, I) to the display. I use it as a migration mechanism for C and Arduino.

**`espzero.ipynb`** - example written for **nodemcu esp8266**, similar 
to `sketch_feb04b`.

**`sketch_feb04b`** sets the display to random digits. Similar to `basic.cc`

## See also

* db5.105
* db5.110 - switches (although they don't seem to work)

## Also on this site

* [pi bare metal](../crunky/examples/14-zeroseg)
* [stm8 bare metal](../stm8s103f3p6/bare/10-max7219)
* [stm32 bare metal](../stm32/bare/14-max7219)


## Links to other sites

* [datasheet](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf) PDF
* [letter representation](https://en.wikichip.org/wiki/seven-segment_display/representing_letters)
