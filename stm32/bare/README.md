# bar metal programming on stm32f1038t6

## delay

This one is pretty good:
```
		for(int i=0; i< 600000; i++) nop(); // simple delay
```

## setup

```
sudo apt install stlink-tools stlink-gui
sudo apt install gcc-arm-none-eabi
```
ARM provide a [toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) that includes a debugger. However, it might be possible to use `gdb-multiarch` instead. It is possible that debugger compatability is tied to the compiler version


## `stlink-tools`

Consists of st-flash, st-info, st-util. To flash a sketch:
```
st-flash write whatever.bin 0x8000000
```
or use `flash-stm32` in the `bin` directory to achieve the same effect. 

If flashing not working:

1. unplug stm
2. set BOOT0 jumper high (the top one)
3. plug in stm
4. `st-flash erase`
5. set BOOT0 jumper low


## Also in this directory

* [blink](blink) - blinky sketch
* [delay](delay) - uses TIM4
* [usart](usart)

* [gpio](gpio.md) - some notes on GPIO

* [reng-spi-master](reng-spi-master) - reverse engineering a SPI master


## Links to other sites
* [Bare Metal STM32 programming - LED blink](https://freeelectron.ro/bare-metal-stm32-led-blink/)
* [datasheet](https://github.com/freeelectron-ro/stm32_playground/tree/master/doc) and ref manual in PDF. Pretty important if you want to do low-level stm32 programming
* [printf](https://github.com/embeddedartistry/printf/blob/89c4cfe96f0340065ad6eb17eeaa508f3eea5f25/printf.c) implementation
* [STM32F10x standard peripheral library](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html)
* [Vivonomicon blog](https://vivonomicon.com/) - contains many STM32 bare metal posts, including the one for UART.
