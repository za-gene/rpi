CC = arm-none-eabi-gcc
CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 
CFLAGS += -nostdlib

OBJS = blue.o gpio.o

libblue.a : $(OBJS)
	ar rcs $@ $^

#blue.o : blue.c blue.h

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f *.o *.a
