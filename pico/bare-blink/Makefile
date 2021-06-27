AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
CFLAGS = -mthumb -mcpu=cortex-m0plus -nostdlib -ggdb
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
LDFLAGS = -T linker.ld

OBJS = crt0.o main.o boot2.o


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
	rm -f *.o app.elf app.list app.bin app.uf2

flash : app.uf2
	cp app.uf2 /media/$(USER)/RPI-RP2

app.uf2 : app.elf app.bin
	$(PICO_SDK_PATH)/build/elf2uf2/elf2uf2 app.elf app.uf2
