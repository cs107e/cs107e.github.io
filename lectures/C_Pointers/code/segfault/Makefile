# Very simple Makefile to build program from single ASM or C file (no link)

all : bogus.bin bogus

ARCH = -march=rv64im -mabi=lp64
CFLAGS = -Og $(ARCH) -Wall -ffreestanding
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
OBJDUMP_FLAGS = --no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps

%.bin: %.o
	riscv64-unknown-elf-objcopy $< -O binary $@

%.o: %.c Makefile
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: bogus.bin
	mango-run $<

# Extra recipe to disassemble the compiled program
list: bogus.o bogus
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d $< >mango.list
	otool -v -t bogus >mac.list

# This recipe is for a native executable bogus hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -O0 -g -Wall $< -o $@

clean:
	rm -rf *.o *.bin *.elf *.list *~ bogus *.dSYM

.PHONY: all clean run list
.PRECIOUS:  %.elf
