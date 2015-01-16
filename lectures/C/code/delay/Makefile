CFLAGS = -O2 -nostdlib -nostartfiles -ffreestanding

blink.bin: blink.c
	arm-none-eabi-gcc $(CFLAGS) start.s blink.c -o blink.o
	arm-none-eabi-objcopy blink.o -O binary blink.bin
	arm-none-eabi-objdump -d blink.o > blink.list

install: blink.bin
	rpi-install.py blink.bin

clean :
	rm -f *.o *.bin *.list


