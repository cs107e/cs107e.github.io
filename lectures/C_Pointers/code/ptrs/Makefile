# This simple Makefile defines pattern rule to build 
# name.bin from name.c C source file

CFLAGS = -Og -g -Wall -std=c99 -ffreestanding
LDFLAGS = -nostdlib -e main
LDLIBS =

all: ptrs ptrs.bin

%.bin: %.elf
	arm-none-eabi-objcopy $*.elf -O binary $@

%.elf: %.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -Wall $< -o $@

clean: 
	rm -rf *.list *.bin *.elf *.o ptrs a.out *.dSYM