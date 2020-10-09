CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -Os
CFLAGS += -I.
CFLAGS += -nostdlib
CFLAGS += -fdata-sections -ffunction-sections # for unused code

OBJS = blue.o gpio.o crt.o i2c.o spi.o usart.o

libblue.a : $(OBJS)
	ar rcs $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.s
	$(AS) -o $@ $<
clean :
	rm -f *.o *.a
