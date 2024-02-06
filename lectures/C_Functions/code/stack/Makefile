NAME = program

all: $(NAME).bin

# Flags for compile and link
ARCH 	= -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS 	= $(ARCH) -g -Og -Wall -ffreestanding -fno-omit-frame-pointer
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS 	=

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: start.o %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

%.list: %.o
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d $<

run: $(NAME).bin
	mango-run $<

debug: $(NAME).elf
	riscv64-unknown-elf-gdb $(GDB_FLAGS) $<

# Remove all build products
clean:
	rm -f *.o *.bin *.elf *.list

.PHONY: all clean run
.PRECIOUS: %.elf %.o

OBJDUMP_FLAGS = --no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps
GDB_FLAGS = -q --command=$$CS107E/other/gdbsim.commands
