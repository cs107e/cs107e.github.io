run:
	riscv64-unknown-elf-as blink.s -o blink.o
	riscv64-unknown-elf-objcopy blink.o -O binary blink.bin
	mango-run blink.bin
