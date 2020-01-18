# Makefile to build program from single ASM or C file
# We will talk more about Makefiles soon!

CFLAGS = -g -Wall -Og -std=c99 -ffreestanding

all : blink.bin c_blink.bin on.bin c_on.bin

%.bin: %.o
	arm-none-eabi-objcopy $< -O binary $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

%.list: %.o
	arm-none-eabi-objdump -d $< >$@

clean:
	rm -f *.o *.bin *~

.PHONY: all clean

.PRECIOUS: %.o
