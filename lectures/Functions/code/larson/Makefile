ARMGNU = arm-none-eabi

larson.bin: larson.s
	cpp -P larson.s | $(ARMGNU)-as -o larson.o
	$(ARMGNU)-objcopy larson.o -O binary larson.bin
	$(ARMGNU)-objdump -d larson.o > larson.list

install: larson.bin
	rpi-install.py larson.bin

clean :
	rm -f *.o *.bin *.list


