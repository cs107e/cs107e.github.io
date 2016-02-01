ARMGNU ?= arm-none-eabi

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img 

all : kernel.img

clean :
	rm -f *.o *.bin *.elf *.list *.img *~ Makefile.bak


start.o : start.s
	$(ARMGNU)-as start.s -o start.o

bootloader.o : bootloader.c rpi.h
	$(ARMGNU)-gcc $(CFLAGS) -c bootloader.c -o bootloader.o

periph.o : periph.c rpi.h
	$(ARMGNU)-gcc $(CFLAGS) -c periph.c -o periph.o

kernel.img : memmap start.o periph.o bootloader.o 
	$(ARMGNU)-ld start.o periph.o bootloader.o -T memmap -o bootloader.elf
	$(ARMGNU)-objdump -D bootloader.elf > bootloader.list
	$(ARMGNU)-objcopy bootloader.elf -O binary kernel.img

depend:
	makedepend *.[ch] 
# DO NOT DELETE

bootloader.o: rpi.h
periph.o: rpi.h
