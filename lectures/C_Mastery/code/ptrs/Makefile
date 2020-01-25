APPLICATION = ptrs
MY_MODULES =

CFLAGS = -g -Og -I$(CS107E)/include -Wall -std=c99 -ffreestanding -Wpointer-arith -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi -lgcc

all : $(APPLICATION).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MY_MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

install: $(APPLICATION).bin
	rpi-install.py -p $<

debug: $(APPLICATION).elf
	arm-none-eabi-gdb -q $<

clean:
	rm -rf *.o *.i *.bin *.elf *.list *~ native*

.PHONY: all clean install

.PRECIOUS: %.o %.elf

# empty recipe used to disable built-in rules for native build
%:%.o
%:%.s

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
native: ptrs.c
	clang -I$(CS107E)/include -g -Wall $< -o $@
