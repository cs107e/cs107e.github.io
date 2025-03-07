ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include -I$(PROJECT_CODE_EXTRAS) -Wall -ffreestanding -fstrict-volatile-bitfields
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lmango -lmango_gcc

all : software.bin hardware.bin

vpath %.c $(PROJECT_CODE_EXTRAS)
hardware.elf: pwm.o

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

software: software.bin
	mango-run $<

hardware: hardware.bin
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean
.PRECIOUS: %.elf %.o
