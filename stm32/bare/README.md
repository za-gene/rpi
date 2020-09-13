# bar metal programming on stm32f1038t6


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
* [de;ay](delay) - uses TIM4
* [usart](usart)

## Links to other sites
* [Bare Metal STM32 programming - LED blink](https://freeelectron.ro/bare-metal-stm32-led-blink/)
* [STM32F10x standard peripheral library](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html)
* [Vivonomicon blog](https://vivonomicon.com/) - contains many STM32 bare metal posts, including the one for UART.
