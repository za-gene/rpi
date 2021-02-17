# blinkt

Turning the blinkt on and off. Low-level implementation is in **blinktlow**.

## Setup

Create menu item Accessories > Pimoroni Dashboard (seems optional):
```
sudo apt-get install pimoroni
```

Install the actual software:
```
sudo apt-get install python3-blinkt
```

## systemd setup

If you want light 0 to be green when the system is running, execute the following:

```
./install-service
sudo systemctl enable blinkt.service
sudo systemctl start  blinkt.service
```

## Wiring

The Blinkt claims to use the SPI interface, but it does not. Orient the device so that the bevelled edge is closest to the case of the RPi.

Pinout diagram:
```
UNO BLI  Notes
5V    2  5V
GND   6  GND
D2   16  BCM23 (data)
D4   18  BCM24 (clock)

Legend:
BLI: Blinkt pin
UNO: Arduino Uno (inc. nano)
```

[Reference](https://pinout.xyz/pinout/blinkt#)

NB: power the device using pin 2, not pin 4, which is also a 5V output on the RPi. A test revealed that you can power it using 3V3, though.

The code supplied on the aforementioned link appears to be wrong, and seems to contain a slight bug. However, its near equivalent, `example01.py`, does the same thing, and can be run by executing the following command:
```
sudo python3 example01.py
```

## Arduino


In Firefox, navigate to 
```
https://github.com/adafruit/Adafruit_DotStar/releases
```
and download the latest release in zip format.

Start the Arduino IDE.

Select menu item Sketch > Include library > Add .ZIP library ..., and select the zip archive you just downloaded. This will add the library under ~/Arduino/libraries/Adafruit_DotStar-1.03.

To test the device, open up the file: `~/Arduino/libraries/Adafruit_DotStar-1.0.3/examples/strandtest/strandtest.ino`. 

If necessary, comment out the line:
```
#include <Adafruit_CircuitPlayground.h>
```

Upload the sketch onto the Arduino. The device should light up.


Aletrnatively (and better), use the blinktlow sketch. The device is highly flexible as to which pins to use.


## ATTiny85

Ensure you have the  Adafruit_Dotstart library installed, as detailed in section "Arduino".

Upload the `strandtest85` sketch to the ATTiny85, using the Arduino as a programmer.

Wire up according to this diagram:

![blinkt85](blinkt85.svg)

## WROOM

Use D2 and D4, as in the Wiring section above for the Uno. Test using sketch blinktlow.

## Also on this site

* [pico-sdk](../pico/blinkt) implementation
