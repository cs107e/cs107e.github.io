PROGRAM = hello.bin

ARCH = -march=rv64im_zicsr -mabi=lp64
ASFLAGS = $(ARCH)
CFLAGS 	= $(ARCH) -g -Og -I$$CS107E/include $$warn $$freestanding
LDFLAGS = $(ARCH) -nostdlib -L$$multidir -L$$CS107E/lib -T memmap
LDLIBS 	= -lmango -lgcc

all : $(PROGRAM)

%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	riscv64-unknown-elf-as $(ASFLAGS) $< -o $@

%.list: %.o
	riscv64-unknown-elf-objdump --no-show-raw-insn -d $< > $@

run: $(PROGRAM)
	mango-run $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean run
.PRECIOUS: %.elf %.o

# disable built-in rules (they are not used)
.SUFFIXES:

# ugh, -ffreestanding _should_ restrict system headers as documented, but doesn't.
# I manually correct below with definition of freestanding to set include
# search path to empty (-nostdinc) and set system include path to the
# correct dir (obtained from gcc)
export freestanding = -ffreestanding -nostdinc -isystem $(shell riscv64-unknown-elf-gcc -print-file-name=include)

export warn = -Wall -Wpointer-arith -Wwrite-strings \
              -Wno-error=unused-function -Wno-error=unused-variable \
              -fno-diagnostics-show-option

export multidir = $(shell dirname `riscv64-unknown-elf-gcc $(ARCH) -print-libgcc-file-name`)

define CS107E_ERROR_MESSAGE
ERROR - CS107E environment variable is not set.

Review instructions for properly configuring your shell.
https://cs107e.github.io/guides/install/userconfig#env

endef

ifndef CS107E
$(error $(CS107E_ERROR_MESSAGE))
endif
