CC = arm-none-eabi-gcc
CFLAGS = -g -Wall -std=c99 -Og -ffreestanding
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

all: gauss.bin
	
%.bin: %.o
	$(OBJCOPY) $< -O binary $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.list: %.o
	$(OBJDUMP) -d $< > $@

install: gauss.bin
	rpi-install.py $<

clean:
	rm -f *.o *.bin
