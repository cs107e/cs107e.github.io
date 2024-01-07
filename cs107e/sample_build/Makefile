PROGRAM = hello.bin

ARCH = -march=rv64imac -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS 	= $(ARCH) -g -Og -I$$CS107E/include -Wall -ffreestanding
LDFLAGS = $(ARCH) -nostdlib -L$$CS107E/lib -T memmap
LDLIBS 	= -lmango -lgcc

all : $(PROGRAM)

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

%.list: %.o
	riscv64-unknown-elf-objdump --no-show-raw-insn -d $< > $@

run: $(PROGRAM)
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean run
.PRECIOUS: %.elf %.o

