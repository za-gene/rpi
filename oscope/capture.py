import timeit
from time import sleep
import spidev
spi = spidev.SpiDev()
#spi.open(0, CHIP_SELECT_0_OR_1)
spi.open(0, 0)
#spi.max_speed_hz = 1000000
spi.max_speed_hz = 1000000

def fetch():
    #to_send = [0x01]
    spi.xfer([0x01])
    dt = 20e-6
    sleep(dt)
    hi, lo = spi.readbytes(2)
    val = (hi << 8) + lo
    #print(val)
    return val

print(timeit.timeit(fetch, number=10000))

