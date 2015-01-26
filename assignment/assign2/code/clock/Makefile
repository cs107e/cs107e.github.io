ARMGNU = arm-none-eabi
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding

clock.bin: clock.c
	$(ARMGNU)-gcc $(CFLAGS) start.s clock.c -o clock.o
	$(ARMGNU)-objcopy clock.o -O binary clock.bin

install: clock.bin
	rpi-install.py clock.bin

clean :
	rm -f *.o *.bin


