NAME = timerblink

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding -I$(CS107E)/include
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi -lgcc

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump -d $< > $@

run: $(NAME).bin
	rpi-run.py -p $<

clean:
	rm -f *.o *.bin *.elf *.list

.PHONY: all clean run

.PRECIOUS: %.o %.elf
