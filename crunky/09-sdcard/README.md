# sdcard



## Test 2 of speed 2021-08-16

Reading a 2,212,397b file.

On qemu, took 513,485us. That's 0.23us/b, or 119us for block of 512b.

On an sdcard with pi0, took 1,394,085us. That's 0.63us/b, or 323us for 512b.

QEMU is 2.7X faster than an actual sdcard.


## Test 1 of speed 2021-08-16


```
         init   512b  4096b
qemu    2.7ms  928us 1339us
card 1   18ms 1607us  673us
card 2    9ms  489us  766us
```

E.g. it took card 1 1607us to read 512b. Note that reading 4096 bytes does not impose significant overhead.

I need to perform CRC tests to ensure that data is being read correctly for 4096b. 
The results are too suspiciously good. A test on qemu produced differing CRC8's, so I'm
figuring that bigger reads won't work.


## Links to other sites

* [#rakulang: let's dump the partition table)(https://mcturra2000.wordpress.com/2020/11/25/rakulang-lets-dump-the-partition-table/)

## Status

2021-08-12	Confirmed working
