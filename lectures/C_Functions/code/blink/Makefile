all : blink.bin

%.bin: %.o
	arm-none-eabi-objcopy $< -O binary $@

%.o: %.s
	arm-none-eabi-as $< -o $@

clean:
	rm -f *.o *.bin

install: blink.bin
	rpi-install.py $<

.PHONY: all clean
