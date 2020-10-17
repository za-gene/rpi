# STM32F103C8T6

Arm Cortex M3, max clock 72MHz, 64KiB flash, 20KiB SRAM

## Arduino IDE

```
Add link:
https://dan.drown.org/stm32duino/package_STM32duino_index.json

In the boards manager search for stm32f1

Uploader:
Generic stm32f103c generic
Upload stlink
```

confirmed working 2020-07-04.

Alternative which does not seem as good:
```
https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json
```

## Unbricking

If all else fails:

1. Unplug the ST-Link
2. Plug it back in again
3. Hold down the reset button
4. In a CLI, issue the command `st-info --probe`
5. Wait until the command completes
6. Release the reset button


## Unclassified

**2020-07-15** I think I **fried** some of the circuits on the board. Both 3.3V out pins near the STLink connectors seems broken. use the one near the USB port instead. I also suspect some of the SPI pins don't work properly.


## Also in this directory

* [adc-interrupts](adc-interrupts) - how to do analog reads using interrupts
* [adc-simple](adc-simple) - just uses the analogRead() function
* [bare-metal](bare) - low-level stuff
* [benchmarks](benchmarks) - how fast does everything work?
* [debugging](debugging.txt)
* song-player - 3V3 SD card, MCP4921 and LED confirmation. Plays raw song.
* stm32-timer-led - using a timer to blink an LED once per second using a timer set a frequency of 8kHz
* [usb-bootloader](usb-bootloader.md) - a bit flakey mind

## Elsewhere in this site

* [sdcard-spi](../sdcard-spi)

## Links to other sites

* [GPIO as output](https://www.gadgetronicx.com/stm32-microcontroller-gpio-output/)
* [GPIO insights](http://embedded-lab.com/blog/stm32-gpio-ports-insights/)
* [interrupts](https://www.electronicshub.org/working-with-interrupts-in-stm32f103c8t6/)
