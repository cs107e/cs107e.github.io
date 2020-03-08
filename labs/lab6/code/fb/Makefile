NAME = main
MODULES = fb.o mailbox.o

CFLAGS = -I$(CS107E)/include -g -Wall -O2 -std=c99 -ffreestanding -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi -lgcc

# The `(OBJECTS:.o=-not-volatile.o)` is notation for: wherever you see something
# in OBJECTS that matches the pattern *.o, replace that with *-not-volatile.o.
# Feel free to use OBJ_NVOL if you'd like to try running the program without the
# `volatile` modifier on the framebuffer or mailbox.
OBJ_NVOL = (MODULES:.o=-not-volatile.o)

all : $(NAME).bin mailbox.list mailbox-not-volatile.list 

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MODULES)
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

# Added `-DNVOLATILE` to define NVOLATILE, found in #ifdefs in fb.c and mailbox.c.
# This tells the preprocessor to include content wrapped between #ifdef and #else,
# as opposed to #else and #endif when NVOLATILE is not defined.
%-not-volatile.o: %.c
	arm-none-eabi-gcc -DNVOLATILE $(CFLAGS) -c $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

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
