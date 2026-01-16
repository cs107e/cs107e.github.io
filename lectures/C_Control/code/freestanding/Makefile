# This simple Makefile defines pattern rule to build 
# name.bin from name.c C source file

ARCH = -march=rv64im -mabi=lp64
CFLAGS 	= $(ARCH) -g -Og -Wall -ffreestanding
LDFLAGS = -nostdlib -e main
LDLIBS =

all: dice dice.bin

%.bin: %.elf
	riscv64-unknown-elf-objcopy $*.elf -O binary $@

%.elf: %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $< -o $@

run: dice.bin
	mango-run $<

list: dice.o dice
	riscv64-unknown-elf-objdump --no-show-raw-insn -d dice.o > dice.list
	otool -v -t dice > dice.mac.list

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
%: %.c
	clang -g -Wall $< -o $@

clean: 
	rm -rf *.list *.bin *.elf *.o dice a.out *.dSYM dice
