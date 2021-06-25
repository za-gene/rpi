AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
CFLAGS = -mthumb -mcpu=cortex-m0plus -nostdlib
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
LDFLAGS = -L.. -T linker.ld

OBJS = main.o crt0.o

app.bin : app.elf
	arm-none-eabi-objcopy -O binary app.elf app.bin
	arm-none-eabi-objdump -d app.elf >app.list

app.elf : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o : %.s
	$(AS) -o $@ $<

clean :
	rm -f *.o app.elf app.list app.bin
