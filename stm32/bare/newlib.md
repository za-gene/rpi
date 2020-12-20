# newlib

The aim is to integrate the libc_nano.a library to provide a standard C library for STM32. 
The STM32F103C8T6 uses an ARM Cortex-M3 core Armv7-M architecture. 
So the library it should be linked against is (unconfirmed):
```
/usr/lib/arm-none-eabi/newlib/thumb/v7-m/libc_nano.a
```

In order to use a function like `sprintf`, `_srk()` must be provided, which is part of `malloc()`.
