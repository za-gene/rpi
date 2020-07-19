import calendar
import datetime
import time
import serial

def send(data):
    ser = serial.Serial(port="/dev/ttyUSB0", baudrate = 9600) 
    print(ser.portstr)       # check which port was really used
    time.sleep(4)
    bin = data.encode('utf-8')
    #print(bin)
    ser.write(bin)
    time.sleep(5)
    ser.close()

def mk_cmd():
    fudge = 5 # to account for delays in uploading
    secs = time.time() + fudge
    ts = datetime.datetime.fromtimestamp(secs)
    gmt = time.gmtime(secs)
    cmd = "SET " + time.strftime("%b %d %Y %H:%M:%S", gmt) +"\r"
    print(cmd)
    return cmd

def main():
    cmd = mk_cmd()
    send(cmd)

if __name__ == "__main__":
    main()
