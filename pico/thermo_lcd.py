# Display on-board temperature on a SPI LCD

import struct
import machine
import utime
#import pyb

def get_temp():
    sensor_temp = machine.ADC(4)
    conversion_factor = 3.3 / (65535)
    reading = sensor_temp.read_u16() * conversion_factor
    temperature = 27 - (reading - 0.706)/0.001721
    print(temperature)
    return temperature

spi_miso=machine.Pin(16)
spi_sck=machine.Pin(18)
spi_mosi=machine.Pin(19)

rs = machine.Pin(20, machine.Pin.OUT)
cs = machine.Pin(17, machine.Pin.OUT)
cs.value(1)

def digitalWrite(pin, val): pin.value(1 if val else 0 )

LOW = 0
HIGH = 1


spi=machine.SPI(0,baudrate=100000,sck=spi_sck, mosi=spi_mosi, miso=spi_miso)

def sendByte(rs_val, val):
    digitalWrite(rs, rs_val);
    digitalWrite(cs, LOW);
    #spi_write_blocking(SPI_PORT, &val, 1);
    #spi.write(struct.pack('b', val))
    spi.write(struct.pack('B', val))
    digitalWrite(cs, HIGH);  
    utime.sleep(0.060);
    
contrast = 0x70  | 0b1000 #; // from 0x7C
display = 0b1111 #; // ori 0x0F
cmds = [0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06]
for c in cmds:
    sendByte(0, c)

msg = ["H", "E", "L", "L", "O"]
#msg = b'HELLO'
def sendStr(msg):
    for b in msg:  sendByte(1, ord(b))
    

def once():
    sendByte(0, 0x01) # clear display
    txt = "{deg:.1f}C    ".format(deg=get_temp())
    sendStr(txt)
    utime.sleep(5)
    
once()    

while True: once()
    
