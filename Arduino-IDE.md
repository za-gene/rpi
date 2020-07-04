# Arduino-IDE

Programming an Attiny85 from an Arduino, esp. wrt ATTiny85.

## Disable checks for updates

File : Preferences : Settings. Uncheck "Check for updates on startup"

## Board Detection `define`s

This is how you detect what board you compiling for.

The upshot is:
```
BOARD  DEFINE
-----  ------------------
WROOM  ARDUINO_ARCH_ESP32 
...
```

So you would use it in your code like so:
```
#ifdef ARDUINO_ARCH_ESP32
#pragma message "Compiling for WROOM"
// etc
#endif
```

More generally, do something like
```
cd ~/.arduino15/packages/esp32/hardware/esp32/1.0.4
grep board= boards.txt | cut -f2 -d= | sort -u
```





## Setup
```
sudo usermod -a -G dialout <username>
sudo chmod a+rw /dev/ttyACM0
sudo chmod a+rw /dev/ttyUSB0
```

IDE 1.8.7 Default programmer is: AVRISP mkll


## Also on this site

* [ATTiny85](attiny85/Arduino-IDE.md) setup and programming instructions
