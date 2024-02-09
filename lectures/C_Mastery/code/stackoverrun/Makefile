PROGRAM = program

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include -Wall -ffreestanding
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango

all : $(PROGRAM).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: $(PROGRAM).bin
	mango-run $<

debug: $(PROGRAM).elf
	riscv64-unknown-elf-gdb $(GDB_FLAGS) $<

clean:
	rm -rf *.o *.bin *.elf *.list *~ *.dSYM program

.PHONY: all clean run
.PRECIOUS: %.elf %.o

GDB_FLAGS = -q --command=$$CS107E/other/gdbsim.commands

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -fno-stack-protector -Wall $< -o $@
