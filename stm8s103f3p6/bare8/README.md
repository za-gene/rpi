# stm8 bare metal programming

Bare metal programming

## library construction notes

There appear to be a number of "gotchas" when trying to create libraries. It is important to try to create the smallest hex files possible. This section presents a number of tips and tricks to that end.

Firstly, compilation modules - basically C files -  are linked in 
on an **all-or-nothing basis**. This means that you should break functional components into separate C files so as to minimise the final code.

The linker needs to **see interrupts** declarations in header files 
(assuming that they are not part of the main program itself) if you break 
the project into several C files. 
Such a C file would best be constructed with a separate header file 
- as in the case of millis. 
Do not incorporate it in with stm8.h, as otherwise, you will find the 
whole module being imported in your project. You might not even want that module.

Take a look at the compiled app.map. You will be able to check that the hex file does not contain any unnecessary features.



## Clues

For clues on implementations, see the following directories:
```
/home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/libraries/I2C
/home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/STM8S_StdPeriph_Driver/src

```


## See also

* [header](https://github.com/junaidpv/stm8/blob/master/inc/stm8s.h) file that "inspired" `stm8.h`
* [pinout](http://www.count-zero.ru/img/stm8/stm8s103f3_pinout.png)
