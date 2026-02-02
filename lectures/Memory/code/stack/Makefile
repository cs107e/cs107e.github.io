NAME = program

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$(CS107E)/include -Wall -ffreestanding -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap.ld
LDLIBS  =

all : $(NAME).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: start.o cstart.o %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

debug: $(NAME).elf
	riscv64-unknown-elf-gdb $(GDB_FLAGS) $<

clean:
	rm -f *.o *.bin *.elf *.list *~ .gdb_scrape

.PHONY: all clean debug
.PRECIOUS: %.elf %.o

GDB_FLAGS = -q --command=$$CS107E/other/gdbsim.commands
