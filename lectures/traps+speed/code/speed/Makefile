ARM = arm-none-eabi
CC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy
TTY = /dev/tty.SLAB_USBtoUART
#FPU_LIB = /opt/gnuarm/lib/gcc/arm-none-eabi/4.8.3/libgcc.a
FPU_LIB = /opt/gnuarm/lib/gcc/arm-none-eabi/4.8.3/libgcc.a
FPU_LIB = /usr/lib/gcc/arm-none-eabi/4.8.2/libgcc.a
PI_LIB = $(LPI)/librpi.a

LPI = ../../../librpi

# Setting DEFAULT sets the default program that make
# will build and install. You can also build specific
# ones to build (e.g. 'make lines-2d.bin').
DEFAULT = timing.bin

LIB_OBJS = helpers.o video.o $(LPI)/start.o
CFLAGS  = -ffreestanding -nostdlib -nostartfiles -Wall -I$(LPI)  -falign-labels=16 -O2
#  -march=armv7-a 

# -march=armv6 -O10

# -O3
#  -falign-labels  -falign-jumps
LDFLAGS = -T $(LPI)/memmap

.PRECIOUS: %.o %.elf

all: lib $(DEFAULT) 

lib:
	make -C $(LPI)

install: all
	rpi-install $(TTY) $(DEFAULT)

.s.o:
	$(AS) $(ASFLAGS) $< -o $@ 

.c.o:
	$(CC) $(CFLAGS) -c $< 

%.elf: $(LIB_OBJS) %.o $(PI_LIB)
	$(LD) $(LDFLAGS) $^ -o $@ $(FPU_LIB)  $(PI_LIB)
	$(OD) -D $@ > $@.list

# /opt/gnuarm/lib/gcc/arm-none-eabi/4.8.3/libgcc.a




%.bin: %.elf
	$(OCP) -O binary $< $@

clean:
	rm -f *.bin *.elf *.list *~ *.o \#* *.bak

depend:
	makedepend *.[ch]
# DO NOT DELETE

opts.o: video.h /usr/include/printf.h /usr/include/features.h
opts.o: /usr/include/stdc-predef.h /usr/include/stdio.h
timing.o: video.h /usr/include/printf.h /usr/include/features.h
timing.o: /usr/include/stdc-predef.h /usr/include/stdio.h
video.o: video.h /usr/include/printf.h /usr/include/features.h
video.o: /usr/include/stdc-predef.h /usr/include/stdio.h
