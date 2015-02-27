ARM = arm-none-eabi
GCC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy

NAME = pwm-square
BIN = $(NAME).bin
LIB_OBJS = start.o cstart.o helpers.o pwm.o led.o system.o timer.o
LIB_OBJS += handlers.o interrupts_asm.o libgcc.a
CFLAGS  = -g -ffreestanding -nostdlib -nostartfiles
LDFLAGS = -T memmap 

.PRECIOUS: %.o %.elf

default: $(BIN)

all: pwm-square.bin pwm-sine.bin pwm-saw.bin pwm-triangle.bin

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
