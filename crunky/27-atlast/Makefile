include $(CRUNKY)/Settings.mk

OBJS = kernel.o atlast.o

atlast: atlast.c atlast-linux.c
	gcc $^ -o $@

include $(CRUNKY)/Germ7.mk
