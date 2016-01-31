all:
	arm-none-eabi-gcc -Wall -std=c99 -Og -ffreestanding -c main.c -o main.o
	arm-none-eabi-gcc -Wall -std=c99 -Og -ffreestanding -c util.c -o util.o
	arm-none-eabi-as start.s -o start.o
	arm-none-eabi-ld -nostdlink main.o util.o start.o -o finished.o

clean:
	rm -f *.o
