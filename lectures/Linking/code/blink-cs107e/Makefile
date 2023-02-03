NAME = blink

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
#CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
ASFLAGS = --warn --fatal-warnings
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
#LDFLAGS = -nostdlib -T memmap 
LDFLAGS += --trace-symbol gpio_init
LDLIBS = -lpi

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o cstart.o start.o
	arm-none-eabi-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

run: $(APPLICATION).bin
	rpi-run.py $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean run
.PRECIOUS: %.elf %.o

