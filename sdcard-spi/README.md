# sdcard-spi

Hardware: SD Card using SPI interface. 

Attach VCC on device to 5V, not 3V£, even on ESP32.

## Nano + IDE

Confirmed working. For example:

Files : Examples : SD : listfiles

MOSI=D11, MISO=D12, CLK=D13, D4




## Wroom + IDE

Confirmed working.

Files : Examples : SD(esp32) : SD_Test

Ignore connection description in ino file, use: SS=5, MOSI=23, CLK=18, MISO=19




## Wroom + FreeRTOS

Could not get this to work, despite various tinkering.



