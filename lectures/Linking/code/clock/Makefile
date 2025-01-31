APPLICATION = clock
MY_MODULES = gpio.o timer.o

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -Wall -ffreestanding
LDFLAGS = -nostdlib -T memmap.ld -no-warn-rwx
LDLIBS  =

#CFLAGS += --save-temps
#LDFLAGS += --trace-symbol gpio_init

all : $(APPLICATION).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o $(MY_MODULES)
	riscv64-unknown-elf-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

%.list: %.o
	riscv64-unknown-elf-objdump -d --no-show-raw-insn $<

clock.elf.list: clock.elf
	riscv64-unknown-elf-objdump -d --no-show-raw-insn $<

run: $(APPLICATION).bin
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~ *.i clock.s cstart.s gpio.s timer.s

.PHONY: all clean run
.PRECIOUS: %.elf %.o
