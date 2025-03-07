ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include -I$(PROJECT_CODE_EXTRAS) -Wall -ffreestanding -fstrict-volatile-bitfields
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango -lmango_gcc

all : dotstar.bin haptic.bin

vpath %.c $(PROJECT_CODE_EXTRAS)
haptic.elf: i2c.o
dotstar.elf: spi.o

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

haptic: haptic.bin
	mango-run $<

dotstar: dotstar.bin
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean
.PRECIOUS: %.elf %.o
