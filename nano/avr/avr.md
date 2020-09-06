# avr

## setup
```
sudo apt install avrdude avrdude-doc avr-libc gcc-avr binutils-avr avrp
```

Possibly consider: dfu-programmer, avra


## serial port

```
git clone https://github.com/tuupola/avr_demo.git
cd avr_demo/blog/simple_usart
make
```

Their upload (`make program`) is a little wrong, so instead type:
```
avrdude -p atmega328p -P /dev/ttyUSB0     -c arduino -b 57600    -U flash:w:main.hex
```

Connect:
```
picocom -b 9600 /dev/ttyUSB0
```


## In this directory

* [hello](hello): simple program to print hello to the serial port 

