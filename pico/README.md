# Pico

## Setup

Define the `PICOMC` environment variable. For example, add this
to `.bashrc`:
```
export PICOMC=/home/$USER/repos/rpi/pico
```


## Technical notes

* [boot](boot.md) - booting into program mode
* [dma](dma.md)
* [gpio](gpio.md)
* [interrupts](interrupts.txt)
* [pwm](pwm.md)
* [serial](serial.md)


## Also in this directory

* [baby](baby.md) 8 sequencer
* [kicad](kicad) - MCU layout for the KiCad electronic circuit design program


## Elsewhere in this repo

* [oled](../1306/pico-sdk)


## Projects

* [freq](freq) - fixed frequency generator using repeating_timer_callback
* [freq-adj](freq-adj)frequency generator whose values can be set up over the USB serial port
* [thermo_lcd.py](thermo_lcd.py) - display on-board temperature to LCD

