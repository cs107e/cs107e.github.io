NAME   = main

all: $(NAME).bin

ARCH    = -march=rv64imafdc -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -O3 -I$$CS107E/include -Wall -ffreestanding -funroll-loops -ffast-math
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango -lmango_gcc

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o math_float.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: $(NAME).bin
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list

.PHONY: all clean run
.PRECIOUS: %.elf %.o
