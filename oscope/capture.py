#!/usr/bin/env python3
import timeit
from time import sleep
import time
import spidev
spi = spidev.SpiDev()
#spi.open(0, CHIP_SELECT_0_OR_1)
spi.open(0, 0)
#spi.max_speed_hz = 1000000
spi.max_speed_hz = 10000

def fetch():
    #to_send = [0x01]
    spi.xfer([0x01])
    dt = 20e-6
    sleep(dt)
    hi, lo = spi.readbytes(2)
    val = (hi << 8) + lo
    #print(val)
    return val

nsamples = 1000
xs = []
ys = []

for i in range(nsamples):
    xs.append(time.monotonic_ns())
    ys.append(fetch())

x0 = xs[0]
for i in range(nsamples):
    xs[i] = (xs[i] - x0)/1000

fp = open("capture.dat", "w")
for i in range(nsamples):
    fp.write(f'{xs[i]}\t{ys[i]}\n')
fp.close()    
print("Capture finished")
