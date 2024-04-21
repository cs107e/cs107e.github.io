APPLICATION = larson
MY_MODULES = gpio.o timer.o

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -Og -g -Wall -ffreestanding
LDFLAGS = -nostdlib -T memmap.ld
LDLIBS  =

#CFLAGS += --save-temps
#LDFLAGS += --trace-symbol gpio_init

all : $(APPLICATION).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o $(MY_MODULES) cstart.o start.o
	riscv64-unknown-elf-ld $(LDFLAGS) $^ $(LDLIBS) -no-warn-rwx -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

%.list: %.o
	riscv64-unknown-elf-objdump -d --no-show-raw-insn $<

larson.elf.list: larson.elf
	riscv64-unknown-elf-objdump -d --no-show-raw-insn $<

run: $(APPLICATION).bin
	mango-run $<

debug: $(APPLICATION).elf
	riscv64-unknown-elf-gdb -q --command=$$CS107E/other/gdbsim.commands $<

clean:
	rm -f *.o *.bin *.elf *.list *~ *.i larson.s cstart.s gpio.s timer.s

.PHONY: all clean debug
.PRECIOUS: %.elf %.o
