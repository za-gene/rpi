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

## Links to other sites

* [Arduino_Scope](https://github.com/Grumpy-Mike/Mikes-Pi-Bakery/tree/master/Arduino_Scope)
* [Let's build an oscilloscope](https://mcturra2000.wordpress.com/2020/07/22/lets-build-an-oscilloscope-with-raspberrypi-and-stm32/) - useful instructions in using this project

