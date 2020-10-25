# sdcard-spi

[Hardware](https://uk.banggood.com/Micro-TF-Card-Memory-Shield-Module-SPI-Micro-Storage-Card-Adapter-p-919914.html?akmClientCountry=GB&rmmds=myorder&cur_warehouse=CN): SD Card using SPI interface. 

## Nano + IDE

Confirmed working. For example:

Files : Examples : SD : listfiles

MOSI=D11, MISO=D12, CLK=D13, SS=D4


* [sketch_jun18c](sketch_jun18c) - SD card speed test on nano


## STM32 + IDE

Confirmed working. See [stm32-arduino-dumpfile](stm32-arduino-dumpfile) 
for test and hookup

See also: [stm32f103-sdcard](https://github.com/NimaMX/STM32F103-SDCARD) for an interesting possibility.

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


