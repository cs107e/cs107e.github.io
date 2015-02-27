ARM = arm-none-eabi
GCC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy

#NAME = pnp-test
NAME = midi-keys
BIN = $(NAME).bin
LIB_OBJS = start.o cstart.o helpers.o gpio.o led.o system.o timer.o
LIB_OBJS += handlers.o interrupts_asm.o libgcc.a midi.o
CFLAGS  = -g -ffreestanding -nostdlib -nostartfiles
LDFLAGS = -T memmap 

.PRECIOUS: %.o %.elf

default: $(BIN)

all: default

install: all
	rpi-install /dev/tty.SLAB_USBtoUART $(BIN)

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $^

%.o: %.s
	$(AS) $^ -o $@

%.elf: $(LIB_OBJS) %.o
	$(LD) $(LDFLAGS) $^ -o $@ 
	$(OD) -D $@ > $@.list

%.bin: %.elf
	$(OCP) -O binary $^ $@

clean:
	rm -f *.bin *.elf *.list *~ *.o \#*
