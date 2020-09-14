# usart

This is a simplified version of the github repo described below.

See [examples](https://github.com/WRansohoff/STM32_UART_Examples). The `echo` project is known to work. In `Makefile`, uncomment:
```
MCU ?= STM32F103C8
```
Also, you'll need to user `UART2`, which is located on pins `PA2` and `PA3`.
[Blog article](https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/)

## Connection

``
       USB STM32
Grey   RXI PA2
White  TXO PA3
```

Also need a common ground
