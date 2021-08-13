# QEMU

This is known to work (2021-08-13) with `08-mini_uart` serial port on stdio:
```
qemu-system-arm -M raspi0 -serial null -serial stdio -bios kernel.img
```

If uart0 is setup then you nedd to reverse ther serial as follows:
```
qemu-system-arm -M raspi0 -serial stdio -serial null -bios kernel.img

```
