ARMGNU ?= arm-none-eabi

ASFLAGS =
CPPFLAGS =
CFLAGS  = -Wall -O2 
CFLAGS += -nostdlib -nostartfiles -ffreestanding
LDFLAGS = 

.PHONY : all clean

.SUFFIXES : 
.SUFFIXES : .c .s .o .exe .bin .list

all : main.bin

clean :
	rm -f *.bin
	rm -f *.exe
	rm -f *.o
	rm -f *.list

main.exe: main.o cstart.o start.o
	$(ARMGNU)-ld $(LDFLAGS) -T memmap $^ -o $@
	$(ARMGNU)-objdump -D $@ > $*.exe.list

.s.o:
	$(ARMGNU)-as $(ASFLAGS) $< -o $@ 
	$(ARMGNU)-objdump -D $@ > $*.list

.c.o:
	$(ARMGNU)-gcc $(CPPFLAGS) $(CFLAGS) -c $< 
	$(ARMGNU)-objdump -D $@ > $*.list

.exe.bin:
	$(ARMGNU)-objcopy $< -O binary $@

