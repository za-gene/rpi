CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3 -msoft-float -ggdb -O0
CFLAGS += -I $(OPENCM3_DIR)/include

all: app.bin

crt.o: crt.s
	$(AS) -o crt.o crt.s

main.o: main.c
	echo $(OPENCM3_DIR)
	$(CC) $(CFLAGS) -c -o main.o main.c

app.elf: linker.ld crt.o main.o
	$(LD) -T linker.ld -o app.elf crt.o main.o

app.bin: app.elf
	$(BIN) -O binary app.elf app.bin

clean:
	rm -f *.o *.elf *.bin

.flash: app.bin
	touch .flash

flash : .flash
	$(STL) write app.bin 0x8000000

erase:
	$(STL) erase


PREFIX?=arm-none-eabi-
CC=$(PREFIX)gcc
OBJCOPY=$(PREFIX)objcopy
OD=bin

#main.o : main.c
#	$(CC) -c main.c
#all: realall.really

SFLAGS= --static -nostartfiles -std=c11 -g3 -Os
SFLAGS+= -fno-common -ffunction-sections -fdata-sections
SFLAGS+= -I./libopencm3/include -L./libopencm3/lib
LFLAGS+=-Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

#M0_FLAGS= $(SFLAGS) -mcpu=cortex-m0 -mthumb -msoft-float
#M0P_FLAGS= $(SFLAGS) -mcpu=cortex-m0plus -mthumb -msoft-float
M3_FLAGS= $(SFLAGS) -mcpu=cortex-m3 -mthumb -msoft-float
#M4FH_FLAGS= $(SFLAGS) -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
#M7SP_FLAGS= $(SFLAGS) -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16
#M7DP_FLAGS= $(SFLAGS) -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16

#include boards.efm32.mk
#include boards.sam.mk
#include boards.stm32.mk
#include boards.ti.mk

realall.really: outdir $(BOARDS_ELF) $(BOARDS_BIN) $(BOARDS_HEX)

libopencm3/Makefile:
	@echo "Initializing libopencm3 submodule"
	git submodule update --init

libopencm3/lib/libopencm3_%.a: libopencm3/Makefile
	$(MAKE) -C libopencm3

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	@#printf "  OBJCOPY $(*).hex\n"
	$(OBJCOPY) -Oihex $(*).elf $(*).hex

outdir:
	mkdir -p $(OD)/efm32
	mkdir -p $(OD)/sam
	mkdir -p $(OD)/stm32
	mkdir -p $(OD)/ti

clean:
	$(RM) $(BOARDS_ELF) $(BOARDS_BIN) $(BOARDS_HEX)

.PHONY: realall.really outdir clean all
$(V).SILENT:
