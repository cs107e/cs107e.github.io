# Simple Makefile to build program from single ASM or C file

all : blink.bin c_blink.bin

ARCH = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS = -Og $(ARCH) -Wall -ffreestanding
LDFLAGS = -nostdlib -e0
OBJDUMP_FLAGS = --no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps=extended-color

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $< -o $@

%.o: %.c Makefile
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

list: c_blink.elf
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d $<

clean:
	rm -f *.o *.bin *.elf *.list *~

run: c_blink.bin
	mango-run $<

.PHONY: all clean run
.PRECIOUS: %.o %.elf
