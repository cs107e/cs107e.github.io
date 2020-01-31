NAME = blink
MODULES = 

CFLAGS = -Og -Wall -std=c99 -ffreestanding -I$(CS107E)/include
ASFLAGS = --warn --fatal-warnings
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o
	arm-none-eabi-ld $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
	arm-none-eabi-gcc -MM $(CFLAGS) -c $< > $*.d

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

install: $(APPLICATION).bin
	rpi-install.py $<

clean:
	rm -f *.a *.o *.d *.bin *.elf *.list *~ 

.PHONY: all clean install

.PRECIOUS: %.o %.elf

# empty recipe used to disable built-in rules for native build
%:%.c
%:%.o
%:%.s

# include dependency info
#  "-" suppresses errors

