# STM32F103C8T6 (Blue Pill) blinker sketch


## setup

```
sudo apt install stlink-tools
sudo apt install gcc-arm-none-eabi
```

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

## Unclassified

**2020-07-15** I think I **fried** some of the circuits on the board. Both 3.3V out pins near the STLink connectors seems broken. use the one near the USB port instead. I also suspect some of the SPI pins don't work properly.


## Also in this directory

* [adc-interrupts](adc-interrupts) - how to do analog reads using interrupts
* [adc-simple](adc-simple) - just uses the analogRead() function
* [benchmarks](benchmarks) - how fast does everything work?
* [debugging](debugging.txt)

## Links to other sites

* [Bare Metal STM32 programming - LED blink](https://freeelectron.ro/bare-metal-stm32-led-blink/)
* [GPIO as output](https://www.gadgetronicx.com/stm32-microcontroller-gpio-output/)
* [GPIO insights](http://embedded-lab.com/blog/stm32-gpio-ports-insights/)
* [interrupts](https://www.electronicshub.org/working-with-interrupts-in-stm32f103c8t6/)
