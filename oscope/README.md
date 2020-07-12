# oscope

Attempts to make an oscilloscope for Raspberry Pi.

## Setup

* For data capture, set up the STM32 [slave unit ](../spi/stm-spi-slave)

## Running

To capture data:
```
python3 capture.py
```
which produces the file `capture.txt`

Plot the data:
```
python3 plot.py
```
