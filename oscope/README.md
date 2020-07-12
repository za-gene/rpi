# oscope

Attempts to make an oscilloscope for Raspberry Pi.

## Setup

* For data capture, set up the STM32 [capture card](capture-card)

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
