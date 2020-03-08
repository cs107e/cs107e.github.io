APPLICATION = names
MY_MODULES = malloc.o

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -Wpointer-arith
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
	arm-none-eabi-as $(ASFLAGS) $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install: $(APPLICATION).bin
	rpi-install.py -s $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean install
.PRECIOUS: %.elf %.o

