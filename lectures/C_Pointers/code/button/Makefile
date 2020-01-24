# Makefile to build program from single ASM or C file

all : c_button.bin button.bin

CFLAGS = -Og -g -Wall -std=c99 -ffreestanding

%.bin: %.o
	arm-none-eabi-objcopy $< -O binary $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump -d $< >$@

clean:
	rm -rf *.list *.bin *.elf *.o

install: c_button.bin
	rpi-install.py $<

.PHONY: all clean