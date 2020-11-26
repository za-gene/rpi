include $(CRUNKY)/Settings.mk


COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)


OBJS = kernel.o


%.o : %.c
	$(ARMGNU)-gcc $(COPS) -c $^ -o $@



include ../../Germ7.mk

