# Set the prefix used for ARM cross-development
ARMGNU = arm-none-eabi

# This will run by default with command 'make'
all:
	$(ARMGNU)-as larson.s -o larson.o
	$(ARMGNU)-objcopy larson.o -O binary larson.bin

# Remove binary files with 'make clean'
clean:
	rm -rf *.o
	rm -rf *.bin
