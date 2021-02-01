# 2021-01-23 confirmed working
# OLED can sometimes be a bit picky

import ssd1306
from machine import I2C, Pin
import utime

width = 128
height = 64

sda = Pin(4) # physical pin 6
scl = Pin(5) # physical pin 7
i2c = I2C(id=0, sda=sda, scl=scl, freq=100000)
ssd = ssd1306.SSD1306_I2C(width, height, i2c)

count =0
def once():
    global count
    ssd = ssd1306.SSD1306_I2C(width, height, i2c)
    ssd.fill(0)
    #ssd.rect(10,10, 10, 10, 1)
    #ssd.show()
    txt = str(count)
    print(txt)
    #ssd.text("      ", 10, 10, 1)
    ssd.text(txt, 10, 10, 1)
    ssd.show()
    count += 1
    utime.sleep(0.5)
    
def loop():
    while True:     once()
    

loop()    
print("Finished")