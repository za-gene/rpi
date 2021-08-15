# QEMU

This is known to work (2021-08-13) with `08-mini_uart` serial port on stdio:
```
qemu-system-arm -M raspi0 -serial null -serial stdio -bios kernel.img
```

If uart0 is setup then you nedd to reverse ther serial as follows:
```
qemu-system-arm -M raspi0 -serial stdio -serial null -bios kernel.img

```

## sdcard

```
-drive format=raw,file=$HOME/disk.img
```

Confirmed working as at 2021-08-15. SD card sizes need to be in powers of 2, e.g. 128 MiB. 
You can use `truncate` to resize your disk image.

## Best so far

```
qemu-system-arm -M raspi0 -serial stdio -serial null -bios kernel.img -drive format=raw,file=$HOME/disk.img
```

