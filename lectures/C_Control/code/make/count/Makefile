# This sample Makefile build a program for the Pi
# from a single C source file
# NAME.c => NAME.o => NAME.elf => NAME.bin
# The recipes use pattern rules that allow you to
# easily change the basename of the file to customize

NAME = count

ARCH = -march=rv64imac -mabi=lp64
CFLAGS 	= $(ARCH) -g -Og -Wall -ffreestanding
LDFLAGS = $(ARCH) -nostdlib -e main

all : $(NAME).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $< -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.bin *.elf *.o

run: $(NAME).bin
	mango-run $<

.PHONY: all clean run
