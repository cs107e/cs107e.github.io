# A simple makefile for building program composed of C/asm source files.
# Builds for ARM target using cross-development toolchain. 
# Refer to CS107E guide to Make for more information on makefiles

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding 
LDFLAGS = -nostdlib -T memmap
LDLIBS = 

all : example.bin framed.bin example.list framed.list

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

framed.o: CFLAGS += -mapcs-frame -fno-omit-frame-pointer

framed.o: example.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@	

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump -d --no-show-raw-insn $< > $@

install: $(NAME).bin
	rpi-install.py $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean

.PRECIOUS: %.o %.elf
