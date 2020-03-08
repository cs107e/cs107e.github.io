NAME = blink

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
ASFLAGS = --warn --fatal-warnings
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o
	arm-none-eabi-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

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
.PRECIOUS: %.elf %.o


# include dependency info
#  "-" suppresses errors

