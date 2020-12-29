# stm8 spi master (blocking)

Status: working

## Hookup

Connect LEDs to pins PD4, PD5, PD6. This allows you inspect output. It shows the lower 3 bits of the results returned by the slave, with PD6 as LSB. The LEDs should therefore count up in binary.

For hookup of SPI: CS=PA3, SCK=PC5, MISO=PC7, MOSI=PC6

A slave counterpart for the STM32 blue pill can be found [here](https://os.mbed.com/users/blippy/code/spi-slave/). Work on an STM8 slave counterpart is ongoing at 
[09-spi-slave](../09-spi-slave).

