# Makefile to build same program to run native and cross-compile

all : bogus.bin bogus

ARCH = -march=rv64im -mabi=lp64
CFLAGS = -Og $(ARCH) -Wall -ffreestanding
LDFLAGS = -nostdlib -e0
OBJDUMP_FLAGS = --no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $< -o $@

%.o: %.c Makefile
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: bogus.bin
	mango-run $<

# Extra recipe to disassemble the compiled program
list: bogus.elf bogus
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d $< >bogus_mango.list
	otool -v -t bogus >bogus_mac.list

# This recipe is for a native executable bogus hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -O0 -g -Wall $< -o $@

clean:
	rm -rf *.o *.bin *.elf *.list *~ bogus *.dSYM

.PHONY: all clean run list
.PRECIOUS:  %.elf
