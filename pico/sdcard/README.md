# SD card raw example

Read SD cards on Pico. Compatible with Maker Pico out of the box.

Use spi interface ("spi1") as follows:
```
SCK  GP10
MOSI GP11
MISO GP12
CS   GP15
```

Limitations:
* file systems not supported, only raw data
* read, but not write, blocks
* single-block read, not multiblock
* version 2 cards supported, not version 1
* hard-coded spi pins


## Status

2021-05-25 Functional

2021-05-23 started. Not working

## References

* [python source](https://github.com/adafruit/Adafruit_CircuitPython_SD/blob/master/adafruit_sdcard.py)
