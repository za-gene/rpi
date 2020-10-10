# usb bootloader

What follows is a flakey set of instructions in installing a bootloader 
to all you to install via USB.


## Flashing the bootloader

Set jumper BOOT0 (nearest the outside of the board) high.
```
st-flash write generic_boot20_pc13.bin 0x0
```
Set jumper BOOT0 low again.


## Uploading sketch

1. Upload method: STM32duino bootloader
2. Port /dev/ttyACM0

Then compile and upload your sketch


## Restoring the original bootloader

If the whole thing is a bust (which is probably will be):
```
st-flash erase
```

## Links to other sites

* [video](https://youtu.be/Tm7IWQLrKYs) explaining the process
