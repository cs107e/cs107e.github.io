all : blink.bin

%.bin: %.o
	riscv64-unknown-elf-objcopy $< -O binary $@

%.o: %.s
	riscv64-unknown-elf-as $< -o $@

clean:
	rm -f *.o *.bin

run: blink.bin
	mango-run $<

.PHONY: all clean
.PRECIOUS: %.o