# This sample Makefile build a program for the Pi 
# from a single C source file
# NAME.c => NAME.o => NAME.elf => NAME.bin
# The recipes use pattern rules that allow you to
# easily change the basename of the file to customize

NAME = count

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -e main

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $< -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.list *.bin *.elf *.o  *.dSYM $(NAME)

install: $(NAME).bin
	rpi-install.py $<

.PHONY: all clean install

# Extra recipe to disassemble the compiled program
list: $(NAME).o $(NAME)
	arm-none-eabi-objdump --no-show-raw-insn -d $< >pi.list
	otool -v -t $(NAME) >mac.list

# Extra recipe to build native executable to run in hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -Wall $< -o $@

