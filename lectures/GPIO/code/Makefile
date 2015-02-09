ARM = arm-none-eabi
GCC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy

NAME = char
BIN = $(NAME).bin
LIB_OBJS = start.o cstart.o helpers.o led.o fb.o gpio.o timer.o gfx.o font.o
CFLAGS  = -g -ffreestanding -nostdlib -nostartfiles
LDFLAGS = -T memmap

.PRECIOUS: %.o %.elf

all: $(BIN) 

install: all
	rpi-install /dev/tty.SLAB_USBtoUART $(BIN)

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
