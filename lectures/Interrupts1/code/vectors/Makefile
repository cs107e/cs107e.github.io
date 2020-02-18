NAME = main
MY_MODULES = vectors.o
BUGGY_MODULES = vectors_buggy.o

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = 


all : $(NAME).elf $(NAME)_buggy.elf $(NAME).list $(NAME)_buggy.list

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MY_MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%_buggy.elf: %.o $(BUGGY_MODULES) start.o cstart.o
		arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

%.list: %.elf
	arm-none-eabi-objdump -d $< > $@

install: $(NAME).bin
	rpi-install.py $<

debug: $(NAME).elf
	arm-none-eabi-gdb -q $^

clean:
	rm -f *.o *.bin *.elf *.list *~

.PHONY: all clean install

.PRECIOUS: %.elf %.o

%:%.c
%:%.o
