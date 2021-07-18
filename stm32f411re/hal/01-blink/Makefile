ARMGNU = arm-none-eabi
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc
CXX = $(ARMGNU)-g++
OBJDUMP = $(ARMGNU)-objdump
OBJCOPY = $(ARMGNU)-objcopy
LD = $(ARMGNU)-ld


CFLAGS =  -I$(HOME)/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS +=  -I/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Core/Include
#CFLAGS += -I/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Include
#/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h
#$(HOME)/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I.

OBJS = main.o


%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
