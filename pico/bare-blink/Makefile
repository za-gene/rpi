AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
CFLAGS = -mthumb -mcpu=cortex-m0plus -nostdlib -ggdb
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
LDFLAGS = -T linker.ld

OBJS = crt0.o main.o

app.bin : app.elf 
	arm-none-eabi-objcopy -O binary app.elf app.bin
	arm-none-eabi-objdump -d app.elf >app.list

app.elf : $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	#$(LD) -o $@ $(OBJS) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o : %.s
	$(AS) -g -o $@ $<

clean :
	rm -f *.o app.elf app.list app.bin
