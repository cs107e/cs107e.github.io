APPLICATION = blink
MODULES = 

CFLAGS = -Og -Wall -std=c99 -ffreestanding -I$(CS107E)/include
ASFLAGS = --warn --fatal-warnings
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi

all : $(APPLICATION).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MODULES) cstart.o start.o
	arm-none-eabi-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

install: $(APPLICATION).bin
	rpi-install.py $<

clean:
	rm -f *.bin *.elf *.o *.list *~ 

.PHONY: all clean install

.PRECIOUS: %.o %.elf

# empty recipe used to disable built-in rules for native build
%:%.c
%:%.o
%:%.s
