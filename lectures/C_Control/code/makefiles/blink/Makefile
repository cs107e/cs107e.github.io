# This very simple Makefile builds blink.bin
# using fixed recipes

blink.bin: blink.s
	riscv64-unknown-elf-as blink.s -o blink.o
	riscv64-unknown-elf-objcopy blink.o -O binary blink.bin

run: blink.bin
	mango-run blink.bin

clean:
	rm -f blink.o blink.bin
