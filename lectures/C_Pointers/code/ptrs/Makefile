CFLAGS = -O2 -g -Wall -std=c99 -ffreestanding
LDFLAGS = -nostdlib -e main

all: ptrs ptrs.bin

%.bin: %.o
	arm-none-eabi-gcc $(LDFLAGS) $< -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -Wall $< -o $@

clean: 
	rm -rf *.bin *.o ptrs a.out *.dSYM