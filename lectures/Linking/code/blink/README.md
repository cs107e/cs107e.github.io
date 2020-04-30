1. run cpp

$ cpp < blink.c
$ arm-none-eabi-gcc -E blink.c -o blink.i

2. run cc1 (gcc -S) to generate assembly language

$ arm-none-eabi-gcc -S blink.c -o blink.s

3. run as

$ arm-none-eabi-gcc -c blink.s -o blink.o

4. run cpp, cc1, and as in one shot, saving all the intermediates.

$ arm-none-eabi-gcc --save-temps -c blink.c -o blink.o

5. linking

$ arm-none-eabi-ld $(LDFLAGS) $^ $(LDLIBS) -o $@

Run with trace-symbol

6.  Build the whole thing using a single gcc command

$ make -f Makefile.all

7 names/symbols

$ arm-none-eabi-nm blink.o
$ arm-none-eabi-nm -n gpio.o
% vim gpio.o.list
$ arm-none-eabi-nm -n blink.elf
% vim blink.elf.list
$ arm-none-eabi-size blink.elf
$ arm-none-eabi-size *.o

8. start.s and memmap

