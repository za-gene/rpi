# Pico

## serial over USB

In `CMakeLists.txt`, after the line
```
add_executable(app main.cc)
```
add the line:
```
pico_enable_stdio_usb(app 1)
```

In `main.cc`:
```
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    ... then e.g. ...
    for(;;) {
        int c = getchar();
        putchar(c);
    }
```

## Technical notes

* [boot](boot.md) - booting into program mode
* [pwm](pwm.md)


## Also in this directory

* [baby](baby.md) 8 sequencer
* [kicad](kicad) - MCU layout for the KiCad electronic circuit design program


## Projects

* [freq](freq) - fixed frequency generator using repeating_timer_callback
* [freq-adj](freq-adj)frequency generator whose values can be set up over the USB serial port
* [thermo_lcd.py](thermo_lcd.py) - display on-board temperature to LCD

