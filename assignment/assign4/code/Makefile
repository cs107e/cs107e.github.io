ARMGNU ?= arm-none-eabi

CPPFLAGS = 
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
LDFLAGS  = 

.PHONY : all clean

.SUFFIXES : 
.SUFFIXES : .c .s .o .exe .bin .list

NAME = hello

all : $(NAME).bin

$(NAME).exe: $(NAME).o db.o printf.o uart.o timer.o gpio.o start.o 
	$(ARMGNU)-ld $(LDFLAGS) -T memmap $^ -o $@
	$(ARMGNU)-objdump -D $@ > $*.exe.list

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

install: $(NAME).bin
	rpi-install.py $(NAME).bin
