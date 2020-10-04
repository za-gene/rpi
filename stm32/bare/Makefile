CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
CFLAGS = -mthumb -mcpu=cortex-m3 -ggdb -O0 
CFLAGS += -nostdlib
CFLAGS += -fdata-sections -ffunction-sections # for unused code

OBJS = blue.o gpio.o crt.o spi.o usart.o

libblue.a : $(OBJS)
	ar rcs $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.s
	$(AS) -o $@ $<
clean :
	rm -f *.o *.a
