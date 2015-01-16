CFLAGS = -O2 -nostdlib -nostartfiles -ffreestanding

on.bin: on.c
	arm-none-eabi-gcc $(CFLAGS) on.c -o on.o
	arm-none-eabi-objcopy on.o -O binary on.bin
	arm-none-eabi-objdump -d on.o > on.list

install: on.bin
	rpi-install.py on.bin

clean :
	rm -f *.o *.bin *.list


