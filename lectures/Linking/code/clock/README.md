PREPROCESSOR cpp (.c -> .i)

$ riscv64-unknown-elf-cpp clock.c
$ riscv64-unknown-elf-gcc -E clock.c -o clock.i

C COMPILER cc1 (gcc -S) to generate assembly language (.c -> .s)

$ riscv64-unknown-elf-gcc -S clock.c -o clock.s

ASSEMBLER

$ riscv64-unknown-elf-as clock.s -o clock.o
$ riscv64-unknown-elf-gcc -c clock.s -o clock.o

SAVE TEMPS run cpp, cc1, and as in one shot, saving all the intermediates (.i, .s)

$ riscv64-unknown-elf-gcc --save-temps -c clock.c -o clock.o

LINKER

$ riscv64-unknown-elf-ld -nostdlib -T memmap.ld  *.o -o clock.elf

SYMBOL TABLE, BINUTILS

$ riscv64-unknown-elf-nm clock.o
$ riscv64-unknown-elf-nm -n gpio.o
$ riscv64-unknown-elf-nm -n clock.elf
$ riscv64-unknown-elf-size clock.elf
$ riscv64-unknown-elf-size *.o

START SEQUENCE

view start.s cstart.c memmap.ld

BUILD ERRORS

cpp: wrong include path, bad directive
cc: semantic/synax, missing include
ld: undefined/multiple define


