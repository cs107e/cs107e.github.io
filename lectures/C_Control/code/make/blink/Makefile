# This very simple Makefile builds blink.bin
# using fixed recipes

blink.bin: blink.s
	arm-none-eabi-as blink.s -o blink.o
	arm-none-eabi-objcopy blink.o -O binary blink.bin

run: blink.bin
	rpi-run.py blink.bin

clean:
	rm -r *.o *.bin
