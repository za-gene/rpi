ARMGNU = arm-none-eabi
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc
CXX = $(ARMGNU)-g++
OBJDUMP = $(ARMGNU)-objdump
OBJCOPY = $(ARMGNU)-objcopy
LD = $(ARMGNU)-ld


%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
