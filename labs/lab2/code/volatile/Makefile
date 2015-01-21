ARMGNU = arm-none-eabi
CFLAGS  = -O2 -nostdlib -nostartfiles -ffreestanding

all:
	$(ARMGNU)-gcc -c $(CFLAGS) volatile.c
	$(ARMGNU)-objdump volatile.o -d > volatile.s

clean:
	rm *.s *.o



