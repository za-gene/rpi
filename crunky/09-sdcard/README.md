# sdcard



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
