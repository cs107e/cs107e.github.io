ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include -Wall -ffreestanding
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld
LDLIBS  = -lextra -lmango -lmango_gcc

all : audio_demo.bin

playwav.c: all-right-now.h

%.h: %.wav
	wav-to-c-header.py $< $@

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

run: audio_demo.bin
	rm -f audio_demo.elf
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~ all-right-now.h

.PHONY: all clean
.PRECIOUS: %.o
