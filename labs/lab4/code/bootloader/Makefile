ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img 

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.img


start.o : start.s
	$(ARMGNU)-as start.s -o start.o

bootloader.o : bootloader.c
	$(ARMGNU)-gcc $(COPS) -c bootloader.c -o bootloader.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : memmap start.o periph.o bootloader.o 
	$(ARMGNU)-ld start.o periph.o bootloader.o -T memmap -o bootloader.elf
	$(ARMGNU)-objdump -D bootloader.elf > bootloader.list
	$(ARMGNU)-objcopy bootloader.elf -O binary kernel.img

