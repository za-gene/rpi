CC = arm-none-eabi-gcc
CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 
CFLAGS += -nostdlib

libblue.a : blue.o
	ar rcs $@ $^

blue.o : blue.c blue.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f *.o *.a
