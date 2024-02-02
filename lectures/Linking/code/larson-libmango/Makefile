APPLICATION = larson

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -Og -g -I$$CS107E/include -Wall -ffreestanding
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango

all : $(APPLICATION).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: $(APPLICATION).bin
	mango-run $<

debug: $(APPLICATION).elf
	riscv64-unknown-elf-gdb -q --command=$$CS107E/other/gdbsim.commands $<

clean:
	rm -f *.o *.bin *.elf *.list *~ *.i blink.s cstart.s gpio.s timer.s

.PHONY: all clean debug
.PRECIOUS: %.elf %.o
