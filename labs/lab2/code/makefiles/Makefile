NAME = blink

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -e main

all: $(NAME).bin
  
%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $< -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
 
install: $(NAME).bin
	rpi-install.py $<

clean:
	rm -f *.o *.elf *.bin