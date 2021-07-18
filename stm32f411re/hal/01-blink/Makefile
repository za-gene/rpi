ARMGNU = arm-none-eabi
AS = $(ARMGNU)-as
CC = $(ARMGNU)-gcc
CXX = $(ARMGNU)-g++
OBJDUMP = $(ARMGNU)-objdump
OBJCOPY = $(ARMGNU)-objcopy
LD = $(ARMGNU)-ld

#LDFLAGS = -T linker.ld

CFLAGS = -mthumb -mcpu=cortex-m4 -nostdlib -ggdb
CFLAGS +=  -I$(HOME)/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS +=  -I/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Core/Include
#CFLAGS += -I/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Include
#/home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h
#CFLAGS += -I$(HOME)/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I.

VPATH = /home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Src
#OBJS = main.o stm32f4xx_hal.o 
OBJS  = main.o
OBJS += stm32f4xx_hal_cortex.o
OBJS += stm32f4xx_hal.o
OBJS += stm32f4xx_hal_gpio.o
#OBJS += stm32f4xx_hal_rcc.o


app.elf : $(OBJS) linker.ld  # stm32f4xx_hal_conf.h
	$(LD)  -o $@ $(OBJS) -T linker.ld

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJS) app.elf
