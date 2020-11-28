# crunky

Attempt to build a "unikernel" for Raspberry Pi 3

## setup

Define the environment variable CRUNKY in your .bashrc file:
```
export CRUNKY=/path/to/crunky
```


## Dissasembly

One of 
```
aarch64-linux-gnu-objdump -D kernel8.elf | less
arm-none-eabi-objdump -D kernel.elf 
```

## In this directory

* [usb](usb) - it's not easy, that's for sure.

## Links to libraries

* [printf](https://github.com/mpaland/printf) - but needs support for math, unfortunately
