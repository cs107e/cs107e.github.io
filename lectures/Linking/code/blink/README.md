1. run cpp

$ cpp < blink.c
$ arm-none-eabi-gcc -E blink.c -o blink.i

2. run cc1

$ arm-none-eabi-gcc -S blink.c -o blink.s

3. run as

$ arm-none-eabi-gcc -c blink.s -o blink.o

4. run cpp, cc1, and as

$ arm-none-eabi-gcc --save-temps -c blink.c -o blink.o

5. make -f Makefile.all

Build the whole thing using a single gcc command



