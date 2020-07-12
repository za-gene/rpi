import spidev
spi = spidev.SpiDev()
#spi.open(0, CHIP_SELECT_0_OR_1)
spi.open(0, 0)
#spi.max_speed_hz = 1000000
spi.max_speed_hz = 10000
to_send = [0x01, 0x02]
#x = spi.xfer(to_send)
#print(x)
hi, lo = spi.readbytes(2)
val = (hi << 8) + lo
print(val)
