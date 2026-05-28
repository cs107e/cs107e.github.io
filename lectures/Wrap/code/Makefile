all: list

OBJDUMP_FLAGS = -d --no-address --no-show-raw-insn
OTOOL_FLAGS =  -v -t -j -X

%.riscv.o: %.c
	riscv64-unknown-elf-gcc -O2 -march=rv64im -mabi=lp64 -c $< -o $@

%.intel.o: %.c
	clang -arch x86_64 -O2 -fomit-frame-pointer -c $< -o $@

%.arm.o: %.c
	clang -arch arm -O2 -c $< -o $@

# Extra recipe to disassemble the compiled program
list: code.riscv.o code.intel.o code.arm.o
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) -d code.riscv.o | fgrep -v "<." >riscv.list
	otool $(OTOOL_FLAGS) code.intel.o >intel.list
	otool $(OTOOL_FLAGS) code.arm.o >arm.list

clean:
	rm -rf *.o *.bin *.elf *.list *~ code

.PHONY: all clean run list
.PRECIOUS:  %.elf
