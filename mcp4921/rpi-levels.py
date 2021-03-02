import spidev, time

def get_level():
    while True:
        try: 
            level = input("Level 0- 4095:")
            level = int(level)
            if level < 0 or level > 4095: raise ValueError
            return level
        except ValueError:
            print("Bad value")

def set_level(spi, level):
    print("Set level: ", level, " (", level*3.3/4096, "V)")
    lo = level & 0xff
    hi = level >> 8
    hi += 0b00110000
    vec = [hi, lo]
    #vec = [lo, hi]
    print(vec)
    spi.xfer(vec)
    #spi.writebytes(vec)


def main():
    try:
        spi = spidev.SpiDev()
        spi.open(0,0)
        spi.max_speed_hz = 1_000_000
        while True:
            #level = get_level()
            level = 200
            set_level(spi, level)
            time.sleep(0.01)
    finally:
        print("Closing spi")
        spi.close()

if __name__ == "__main__":
    main()
