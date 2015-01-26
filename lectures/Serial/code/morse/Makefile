ARMGNU ?= arm-none-eabi

CPPFLAGS = -I../include
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
LDFLAGS  = -L../lib
LIBS = -lpi

.PHONY : all clean

.SUFFIXES : 
.SUFFIXES : .c .s .o .exe .bin .list

NAME = sos

all : $(NAME).bin

$(NAME).exe: $(NAME).o start.o 
	$(ARMGNU)-ld $(LDFLAGS) start.o $(NAME).o -o $@ $(LIBS)
	$(ARMGNU)-objdump -D $@ > $*.list

.s.o:
	$(ARMGNU)-as $(ASFLAGS) $< -o $@ 

.c.o:
	$(ARMGNU)-gcc $(CPPFLAGS) $(CFLAGS) -c $< 

.exe.bin:
	$(ARMGNU)-objcopy $< -O binary $@

clean :
	rm -f *.o
	rm -f *.exe
	rm -f *.bin
	rm -f *.list

install:
	rpi-install.py $(NAME).bin
