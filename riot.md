# riot rtos

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

