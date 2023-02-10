CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi -lgcc

all: bogus.bin bogus

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

run: bogus.bin
	rpi-run.py -p $<

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -Wall $< -o $@

clean:
	rm -rf *.o *.bin *.elf *.list *~ bogus *.dSYM

.PHONY: all clean run list
.PRECIOUS:  %.elf
