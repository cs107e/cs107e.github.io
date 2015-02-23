ARM = arm-none-eabi
GCC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy

NAME = interrupt_display
BIN = $(NAME).bin
LIB_OBJS = start.o cstart.o helpers.o led.o fb.o gpio.o system.o timer.o gfx.o font.o
LIB_OBJS += handlers.o interrupts_asm.o
CFLAGS  = -g -ffreestanding -nostdlib -nostartfiles
LDFLAGS = -T memmap

.PRECIOUS: %.o %.elf

all: $(BIN) 

install: all
	rpi-install /dev/tty.SLAB_USBtoUART $(BIN)

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $^

%.o: %.s
	$(AS) $^ -o $@

%.elf: $(LIB_OBJS) %.o
	$(LD) $(LDFLAGS) $^ -o $@ 
	$(OD) -D $@ > $@.list

console.elf: console.o libpal.a start.o cstart.o
	$(LD) $(LDFLAGS) console.o libpal.a start.o libgcc.a  -o console.elf
	$(OD) -D console.elf > console.elf.list

console.bin: console.elf
	$(OCP) -O binary $^ $@

%.bin: %.elf
	$(OCP) -O binary $^ $@

clean:
	rm -f *.bin *.elf *.list *~ *.o \#*
