# riot rtos

## Arduino Nano

Onboard LED is "A5", which you can set as:
```
#define EXTERNAL_LED GPIO_PIN(2,5)
```


## STM32 blue pill


### Serial

Default speed is 115200 baud.

The for the BluePill the printf statements go the the `STDIO` at `UART_DEV(1)` 
which resides at pins `PA2` (TX2) and `PA3`(RX2).

`STDIN` has to be enabled in the Makefile:
```
USEMODULE += stdin
```

To build a simple example:
```
cd riot/examples/hello-world
BOARD=bluepill make
```

According to [this](https://forum.riot-os.org/t/serial-port-not-working-on-stm32-bluepill/3278/3):
> So for BluePill this is: …/RIOT/boards/common/blxxxpill/include There you will find the actual used device. In board_common.h you will see that the STDIO output is assiged for UART 2 - resources start counting from 0 - on the Bluepill. The pinout diagram tells you which physical pins. Keep this location in mind if you plan to use more of the Bluepill internal devices!


## xtimer (delay)

If you use delay, add to `main.c`:
```
#include "xtimer.h"        // Use for create the delay
...
int main(void)
{
	xtimer_init();
	...
	xtimer_msleep(100); // milliseconds
	...
	return 0;
}
```

See also: [blog](https://mcturra2000.wordpress.com/2021/06/22/kicking-the-tyres-of-riot-os/)


## Links to other sites

* [documentation](file:///home/pi/riot/doc/doxygen/html/index.html) - local
