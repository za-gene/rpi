# sdcard-spi

Hardware: SD Card using SPI interface. 

Attach VCC on device to 5V, not 3V3, even on ESP32.

## Nano + IDE

Confirmed working. For example:

Files : Examples : SD : listfiles

MOSI=D11, MISO=D12, CLK=D13, SS=D4




## Wroom + IDE

Confirmed working.

Files : Examples : SD(esp32) : SD_Test

Ignore connection description in ino file, use: SS=D5, MOSI=D23, CLK=D18, MISO=D19




## Wroom + FreeRTOS

Could not get this to work, despite various tinkering.


## Wroom + MicroPython

Doesn't seem to work if the instructions below are followed.

Upload [sdcard.py](https://github.com/micropython/micropython/blob/master/drivers/sdcard/sdcard.py):

```
rshell --buffer-size=30 -p /dev/ttyU* cp sdcard.py /pyboard

```

Run `wroom-micropython-example.ipynb` . 

