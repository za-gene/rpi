# SPI receiving slave 

Receives a `uint32_t` counter value and displays it on a 7-segment display. 

Uses interrupts on GPIO pins to do the slave operation. The SDK/hardware
seems to have some wrong/bad assumptions about how slave SS pins should work.

## See also

* **db05.316** for a pre-made device. It seems a little finnicky with the
display, so if it's not working, unplug it, wait a few seconds, and plug it back
in again. The problem does not appear to be with the interrupt mechanism, though.
** 43-spi-slave-tx for the counterpart of this project

## Status

2021-08-03	Started. Working.
