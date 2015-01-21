ARMGNU = arm-none-eabi
CFLAGS  = -O2 -nostdlib -nostartfiles -ffreestanding

all:
	$(ARMGNU)-gcc -c $(CFLAGS) opt.c
	$(ARMGNU)-objdump opt.o -d > opt.s

clean:
	rm *.s *.o



