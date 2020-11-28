include $(CRUNKY)/Settings.mk


COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)


OBJS = kernel.o sd-test.o


%.o : %.c
	$(CC) $(COPS) -c $^ -o $@



include ../../Germ7.mk

