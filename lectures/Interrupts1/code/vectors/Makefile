NAME = main
MY_MODULES = vectors.o
BRANCH_MODULES = vectors_branch.o
LDR_MODULES = vectors_ldr.o
LDR2_MODULES = vectors_ldr2.o interrupts_ldr2.o

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi

all : $(NAME).elf $(NAME)_branch.elf $(NAME)_ldr.elf $(NAME)_ldr2.elf $(NAME).list $(NAME)_branch.list $(NAME)_ldr.list $(NAME)_ldr2.list

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MY_MODULES) 
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%_branch.elf: %.o $(BRANCH_MODULES) 
		arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%_ldr.elf: %.o $(LDR_MODULES) 
		arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%_ldr2.elf: %.o $(LDR2_MODULES) 
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

