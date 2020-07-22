#!/usr/bin/env python3
import timeit
from time import sleep
import time
import spidev
spi = spidev.SpiDev()
#spi.open(0, CHIP_SELECT_0_OR_1)
spi.open(0, 0)
spi.max_speed_hz = 1000000
spi.max_speed_hz =  500000

def fetch():
    #to_send = [0x01]
    hi, lo = spi.xfer([0x01, 0x02])
    #dt = 1e-6
    #dt = 0
    #sleep(dt)
    #hi, lo = spi.readbytes(2)
    val = (hi << 8) + lo
    #print(val)
    return val

nsamples = 100
xs = []
ys = []

fetch() # discard buffered reading
for i in range(nsamples):
    xs.append(time.monotonic_ns()/1000)
    ys.append(fetch())
    #sleep(1e-6)

x0 = xs[0]
for i in range(nsamples):
    xs[i] = (xs[i] - x0)

fp = open("capture.dat", "w")
for i in range(nsamples):
    fp.write(f'{xs[i]}\t{ys[i]}\n')
fp.close()    
print("Capture finished")
