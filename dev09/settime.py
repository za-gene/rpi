import time
import serial
ser = serial.Serial(port="/dev/ttyUSB0", baudrate = 9600)  # open first serial port
print(ser.portstr)       # check which port was really used
time.sleep(7)
data = b"h"
ser.write(data)      # write a string
#ser.write(0xa4)         # write a byte
time.sleep(5)
ser.close()             # close port
