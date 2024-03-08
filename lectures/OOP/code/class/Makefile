# assign SRC_PROGRAMS = binky.c and include common Makefile and you're good!

SRC_PROGRAMS = test.cpp 

START_OBJS = A.o

PRODUCTS = $(SRC_PROGRAMS:.cpp=.bin)

# Flags for compile and link
ARCH    = -march=rv64im -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS  = $(ARCH) -g -Og $$warn $$freestanding -fno-omit-frame-pointer
LDFLAGS = -nostdlib 
LDLIBS  = 
OBJDUMP_FLAGS = -d 
#OBJDUMP_FLAGS = -d -no-show-raw-insn --no-addresses --disassembler-color=terminal --visualize-jumps


all :: $(PRODUCTS)

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o $(START_OBJS)
	riscv64-unknown-elf-g++ $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.cpp
	riscv64-unknown-elf-g++ $(CFLAGS) -c $< -o $@

%.s: %.cpp
	riscv64-unknown-elf-g++ $(CFLAGS) -S $< -o $@


%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

%.list: %.o
	riscv64-unknown-elf-objdump $(OBJDUMP_FLAGS) $<

run: $(firstword $(PRODUCTS))
	mango-run $<

clean::
	rm -f *.o *.s *.bin *.elf *.list *~

.PHONY: all clean run
.PRECIOUS: %.elf %.o

# ugh, -ffreestanding _should_ restrict system headers as documented, but doesn't.
# I manually correct below with definition of freestanding to set include
# search path to empty (-nostdinc) and set system include path to the
# correct dir (obtained from g++)
export freestanding = -ffreestanding -nostdinc -isystem $(shell riscv64-unknown-elf-g++ -print-file-name=include)

export warn = -Wall -Wpointer-arith -Wwrite-strings \
              -Wno-error=unused-function -Wno-error=unused-variable \
              -fno-diagnostics-show-option
