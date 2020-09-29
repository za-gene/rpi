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
STM32  MCU_STM32F103C8
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

## Boards

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
https://dl.espressif.com/dl/package_esp32_index.json
https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json
https://github.com/tenbaht/sduino/raw/master/package_sduino_stm8_index.json
https://dan.drown.org/stm32duino/package_STM32duino_index.json
```



## Setup
```
sudo usermod -a -G dialout $USER
sudo chmod a+rw /dev/ttyACM0
sudo chmod a+rw /dev/ttyUSB0
./arduino-linux-setup.sh $USER
```

IDE 1.8.7 Default programmer is: AVRISP mkll

## Unclassified

There is a `String` class, which behaves a little like `std::string`. See
```
~/tools/arduino-1.8.10/hardware/arduino/avr/cores/arduino/WString.cpp
```


## Also on this site

* [ATTiny85](attiny85/Arduino-IDE.md) setup and programming instructions
* [stm32](../stm32/README.md) setup and programming instructions

## Links to other sites

* [HeliOS](https://github.com/MannyPeterson/HeliOS) - embedded OS
* [Reference](file:///home/pi/tools/arduino-1.8.10/reference/www.arduino.cc/en/Reference/HomePage.html)
