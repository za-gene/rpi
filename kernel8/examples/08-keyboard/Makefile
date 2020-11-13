#ARMGNU ?= arm-none-eabi
ARMGNU ?= arm-linux-gnueabihf
CC = arm-linux-gnueabihf-gcc-8
CC = aarch64-linux-gnu-gcc



SmartStart64.o : SmartStart64.S
	$(CC)  -c $< -o $@


OBJS = rpi-usb.o rpi-SmartStart.o SmartStart64.o
include ../../Germ7.mk
