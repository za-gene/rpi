CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 
CFLAGS += -nostdlib

OBJS = blue.o gpio.o crt.o usart.o

libblue.a : $(OBJS)
	ar rcs $@ $^

#blue.o : blue.c blue.h

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.s
	$(AS) -o $@ $<
clean :
	rm -f *.o *.a
