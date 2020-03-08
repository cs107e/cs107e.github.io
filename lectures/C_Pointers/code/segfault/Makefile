CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -e main

all: bogus.bin bogus

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

install: bogus.bin
	rpi-install.py $<

# Extra recipe to disassemble the compiled program
list: bogus.o bogus
	arm-none-eabi-objdump --no-show-raw-insn -d $< >pi.list
	otool -v -t bogus >mac.list

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -Wall $< -o $@

clean: 
	rm -rf *.o *.bin *.elf *.list *~ bogus *.dSYM

.PHONY: all clean install list
