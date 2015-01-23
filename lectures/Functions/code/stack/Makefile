ARMGNU ?= arm-none-eabi

CFLAGS  = -Wall -nostdlib -nostartfiles -ffreestanding  
CFLAGS += -O2

func.list: func.c
	$(ARMGNU)-gcc $(CFLAGS) -c func.c
	$(ARMGNU)-objdump func.o -d > func.list

clean :
	rm -f *.o
	rm -f *.list





