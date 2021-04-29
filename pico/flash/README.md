# Flash

Non-volatile storage on flash. The example isn't perfect, so please feel free to
suggest improvements.

Build the program, and flash it to the chip. It looks for the magic number
"DATA" in the area set aside for storage. Initially, it won't be able to find
it, so it just uses the value of the `data` structure.

The data structure is written to flash every time the program is run. When you
first run it, the counter will output 1 on the serial. Power the Pico off and
on again. Reconnect to the serial, and the counter should have incremented by 1
on each reboot. This proves that memory has been stored in a non-volatile region
of flash.

It took a lot of effort for me to get this poxy thing working! Here are some
gotchas to be aware of in `write_flash_data()`:
*  `save_and_disable_interrupts()` and `restore_interrupts` may or may not
be necessary. It was necessary in this instance because USB serial was used.
Apparently this is because the USB handler uses an interrupt that is in
flash. It says in the SDL description for `hardware_flash`: "these 
functions (`flash...()`) are unsafe if you have two cores concurrently 
executing from flash ... 
Likewise they are unsafe if you have interrupt handlers or an interrupt 
vector table in flash"
* Counter-intuitively, `flash_range_erase()` needs to be called if
you are going to do a write via `flash_range_program()`. Apparently,
flash only allows bit to be set to zero on writes. You therefore need to
force them to be all ones so that those you want to be ones when written
are left as ones.

There matters are discussed (here)[https://www.raspberrypi.org/forums/viewtopic.php?f=145&t=310689&sid=30df829bf12a3edd977fb2aa7692e2ed] and
(here)[https://www.raspberrypi.org/forums/viewtopic.php?p=1854403#p1854403]. 



## Status

2021-04-29 Working.

2021-04-28 Started. Non-working.

