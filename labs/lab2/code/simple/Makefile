ARMGNU = arm-none-eabi
CFLAGS  = -O2 -nostdlib -nostartfiles -ffreestanding

all:
	$(ARMGNU)-gcc -c $(CFLAGS) simple.c
	$(ARMGNU)-objdump simple.o -d > simple.s

clean:
	rm *.s *.o



