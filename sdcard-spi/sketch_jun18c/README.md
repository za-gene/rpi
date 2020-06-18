# SD card speed test on an Arduino Nano

This experiment was to see how long it took to read 8K bytes from an SD Card. I had in mind that I wanted to create a music player with an Arduino with a sample rate of 8Hz and 8 bits per sample. A Nano is interchangeable with an Arduino Uno.

Summary Results 

```
BLK    TIME    X
---  ------ ----
1    307296 1.00
8     75288 4.08
16    59956 5.12
256   45688 6.72
512   37848 8.12
```
Legend:

* `BLK` is the size of each block read. 
* `TIME` is the time taken to transfer 8196 bytes (8Kby) in us (microseconds). 
* `X` is the speed-up time as the result of block transfers relative to transferring 1 byte at a time (the first row is the base value of 1.00)

As can be seen, transferring 1 byte at a time from the SD card took 307296us, i.e. 0.31s for 8K. 8K represents approx 8kHz, but more accurately, the transfer time for 8kHz was 0.30s (=307296*8000/8196 us).

This might not seem much, but it means that for every 1s of play time, 0.3s is taken up retrieving data from the card. That leaves only 0.7s every second to perform other operations.

By transferring 8 bytes at a time, the transfer rate is sped up 4.08 times (=307296/75288). Transfer time is therefore only 0.07s (=0.3/4.08), leaving 0.93s to perform other tasks; a considerable improvement.

As the block size increases to 512bytes, the transfer time gets considerably shorter. However, using 512 bytes as a data buffer uses much of the Arduino's memory, so a lower block size is recommended.

The [source](source.ino) file that produces the timings is less than 70 lines long. It assumes that the SD card has a file on it called `song.raw` which is at least 8196 bytes long.

The wiring is as follows:

```
CARD  NANO
----  ----
MISO   D12
MOSI   D11
SCK    D13
CS     D04
```


The code defines `SD_CS` for `CS`, but you can choose your own pin for the chip select. The other pins are standard SPI pins.
