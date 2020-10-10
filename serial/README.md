# serial

**Logging**
```
picocom /dev/ttyUSB1 -b 115200 -l | tee my.log
```
or
```
picocom -b 115200 /dev/ttyUSB0 -g my_picocom_logfile.txt
```


## stm32

[serial-out](https://ide.mbed.com/compiler/#nav:/serial-out;) contains mbed code for producing serial out, using PA_9 for TX, and PA_10 for RX

The USB can be used for serial communication via `Serial` in the Arduino IDE. See
[stm32-hello-arduino](stm32-hello-arduino). A CP102 chip is used (I believe) to
perform the communication.
The pins used are PA11 (USB-) and PA12 (USB+).

## Also in this directory

* [esp32-proxy](esp32-proxy) - forwards serial communications (e.g. from stm32). Use pins labelled RX2 and TX2 on WROOM
* [picocom](picocom.md)

## Elsewhere in this project

* [stty](../0clock/0clock-1) for clues about automating with serial port
