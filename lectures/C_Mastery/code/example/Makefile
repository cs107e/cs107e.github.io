APPLICATION = example

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -Wpointer-arith -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi -lgcc

all : $(APPLICATION).bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install: $(APPLICATION).bin
	rpi-install.py -p $<

clean:
	rm -rf *.o *.bin *.elf *.list *~ native*

.PHONY: all clean install
.PRECIOUS: %.elf %.o

# This recipe is for a native executable on hosted environment
# (not cross-compiled, not freestanding)
native: example.c
	clang -I$(CS107E)/include -g -fno-stack-protector -Wall $< -o $@
