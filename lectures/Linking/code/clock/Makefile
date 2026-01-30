APPLICATION = clock
MY_MODULES = gpio.o timer.o

ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$(CS107E)/include -Wall -ffreestanding
LDFLAGS = -nostdlib -T memmap.ld
LDLIBS  =

#CFLAGS += --save-temps
#LDFLAGS += --trace-symbol gpio_init

all : $(APPLICATION).bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: start.o cstart.o %.o $(MY_MODULES)
	riscv64-unknown-elf-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

list: clock.elf
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d $<

run: $(APPLICATION).bin
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~ *.i clock.s cstart.s gpio.s timer.s

.PHONY: all clean run
.PRECIOUS: %.elf %.o
OBJDUMP_FLAGS = --no-show-raw-insn
