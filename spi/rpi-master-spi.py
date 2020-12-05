import spidev, time
spi = spidev.SpiDev()

# notes about transferring multiple values
# to_send = [0x01, 0x02]
# hi, lo = spi.readbytes(2)
# val = (hi << 8) + lo
# print(val)

def loop():
    x = spi.xfer([0])
    print(x)
    time.sleep(1.0)


def main():
    global spi
    try:
        spi.open(0, 0)
        spi.max_speed_hz = 100_000
        while True:
            loop()
    finally:
        spi.close()

if __name__ == "__main__":
    main()