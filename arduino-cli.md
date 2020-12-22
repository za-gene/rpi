# arduino-cli

## install and setup

```
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh

```

`arduino:avr` might no be installed.  See what's available:
```
arduino-cli core search arduino
```

Then install it (35M):
```
arduino-cli core install arduino:avr
```

Confirm installation of nano:
```
arduino-cli board listall | grep -i avr
```
outputs:
```
...
Arduino Nano                         arduino:avr:nano   
...
```




## dunno

```
arduino-cli config init

arduino-cli board listall

arduino-cli board list

arduino-cli compile --fqbn stm32duino:STM32F1:genericSTM32F103C

# for stm32: not working
arduino-cli upload --port /dev/ttyACM0 --fqbn stm32duino:STM32F1:genericSTM32F103C

arduino-cli upload --port /dev/cu.usbmodem14201 --fqbn arduino:samd:mkr1000 MKR1000-DHT11/



```

## Links to other sites

* [Arduino-cli](https://www.survivingwithandroid.com/arduino-cli-compile-upload-manage-libraries-cores-boards/)

