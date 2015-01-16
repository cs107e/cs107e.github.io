CFLAGS = -O2 -nostdlib -nostartfiles -ffreestanding

sos.bin: sos.c
	arm-none-eabi-gcc $(CFLAGS) start.s sos.c -o sos.o
	arm-none-eabi-objcopy sos.o -O binary sos.bin
	arm-none-eabi-objdump -d sos.o > sos.list

install: sos.bin
	rpi-install.py sos.bin

clean :
	rm -f *.o *.bin *.list


