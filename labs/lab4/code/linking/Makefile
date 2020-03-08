NAME = linking

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi

all : $(NAME).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@
	arm-none-eabi-objdump --no-show-raw-insn -d $@ > $@.list

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
	arm-none-eabi-objdump --no-show-raw-insn -d $@ > $@.list

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@
	arm-none-eabi-objdump --no-show-raw-insn -d $@ > $@.list

install: $(NAME).bin
	rpi-install.py -p $<

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean install
.PRECIOUS: %.elf %.o

# empty recipe used to disable built-in rules for native build
%:%.c
%:%.o

define CS107E_ERROR_MESSAGE
ERROR - CS107E environment variable is not set.

Review instructions for properly configuring your shell.
https://cs107e.github.io/guides/install/userconfig#env

endef

ifndef CS107E
$(error $(CS107E_ERROR_MESSAGE))
endif

