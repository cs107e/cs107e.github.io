# This sample Makefile build a program for the Pi
# from a single C source file
# NAME.c => NAME.o => NAME.elf => NAME.bin
# The recipes use pattern rules that allow you to
# easily change the basename of the file to customize

NAME = count

ARCH    = -march=rv64im -mabi=lp64
CFLAGS 	= $(ARCH) -g -Og -Wall -ffreestanding
LDFLAGS = $(ARCH) -nostdlib -e main
OBJDUMP_FLAGS = --no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps

all : $(NAME).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $< -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

list: $(NAME).o
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d $<

clean:
	rm -f *.bin *.elf *.o *.list

run: $(NAME).bin
	mango-run $<

.PHONY: all clean run list
