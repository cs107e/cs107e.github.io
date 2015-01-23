ARMGNU = arm-none-eabi

blink.bin: blink.s
	$(ARMGNU)-as blink.s -o blink.o
	$(ARMGNU)-objcopy blink.o -O binary blink.bin
	$(ARMGNU)-objdump -d blink.o > blink.list

install: blink.bin
	rpi-install.py blink.bin

clean :
	rm -f *.o *.bin *.list


