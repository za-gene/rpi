# 2021-01-23 confirmed working
# OLED can sometimes be a bit picky

import ssd1306
from machine import I2C, Pin
sda = Pin(4) # physical pin 6
scl = Pin(5) # physical pin 7
i2c = I2C(id=0, sda=sda, scl=scl, freq=100000)
ssd = ssd1306.SSD1306_I2C(128, 64, i2c)

ssd.rect(10,10, 10, 10, 1)
ssd.show()
ssd.text("hello", 10, 10, 1)
ssd.show()
print("Finished")