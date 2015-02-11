ARM = arm-none-eabi
GCC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy
TTY = /dev/tty.SLAB_USBtoUART

# Setting DEFAULT sets the default program that make
# will build and install. You can also build specific
# ones to build (e.g. 'make lines-2d.bin').
DEFAULT = 

LIB_OBJS = start.o cstart.o helpers.o led.o fb.o font.o
CFLAGS  = -g -ffreestanding -nostdlib -nostartfiles -O3
LDFLAGS = -T memmap

.PRECIOUS: %.o %.elf

all: $(DEFAULT) 

install: all
	rpi-install $(TTY) $(DEFAULT)

%.o: %.c
	$(ARM)-gcc $(CFLAGS) -c -o $@ $^

%.o: %.s
	$(AS) $^ -o $@

%.elf: $(LIB_OBJS) %.o
	$(LD) $(LDFLAGS) $^ -o $@ 
	$(OD) -D $@ > $@.list

%.bin: %.elf
	$(OCP) -O binary $^ $@

clean:
	rm -f *.bin *.elf *.list *~ *.o \#*
