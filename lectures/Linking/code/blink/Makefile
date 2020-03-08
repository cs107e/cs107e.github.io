APPLICATION = blink
MODULES = timer.o gpio.o

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
CFLAGS += --save-temps

ASFLAGS = --warn --fatal-warnings

LDFLAGS = -nostdlib -T memmap 
LDFLAGS += --trace-symbol gpio_init

all : $(APPLICATION).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MODULES) cstart.o start.o
	arm-none-eabi-ld $(LDFLAGS) $^ $(LDLIBS) -o $@
	arm-none-eabi-objdump -d $@ > $@.list

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
	arm-none-eabi-objdump -d $@ > $@.list

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@
	arm-none-eabi-objdump -d $@ > $@.list

install: $(APPLICATION).bin
	rpi-install.py $<

clean:
	rm -f *.o *.i *.bin *.elf *.list *~ blink.s cstart.s gpio.s timer.s 

.PHONY: all clean install
.PRECIOUS: %.elf %.o

