all : blink.bin

%.bin: %.o
	arm-none-eabi-objcopy $< -O binary $@

%.o: %.s
	arm-none-eabi-as $< -o $@

clean:
	rm -f *.o *.bin

run: blink.bin
	rpi-run.py $<

.PHONY: all clean
