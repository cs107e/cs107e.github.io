#
# This makefile doesn't build a program to run.
# Its only target is the `codegen.list` file
# that allows you to examine the disassembly from the compiler.
#

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding

all : codegen.list 

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

clean:
	rm -f *.o *.bin *.elf *.list

.PHONY: all clean
