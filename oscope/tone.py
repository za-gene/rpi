#!/usr/bin/env python3
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(35, GPIO.OUT)
pwm35 = GPIO.PWM(35, 440)
pwm35.start(50)
input('Press return to stop:') #Wait
pwm35.stop()
GPIO.cleanup()
