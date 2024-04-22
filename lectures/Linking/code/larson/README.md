PREPROCESSOR cpp (.c -> .i)

$ riscv64-unknown-elf-cpp larson.c
$ riscv64-unknown-elf-gcc -E larson.c -o larson.i

C COMPILER cc1 (gcc -S) to generate assembly language (.c -> .s)

$ riscv64-unknown-elf-gcc -S larson.c -o larson.s

ASSEMBLER

$ riscv64-unknown-elf-as larson.s -o larson.o
$ riscv64-unknown-elf-gcc -c larson.s -o larson.o

SAVE TEMPS run cpp, cc1, and as in one shot, saving all the intermediates (.i, .s)

$ riscv64-unknown-elf-gcc --save-temps -c larson.c -o larson.o

LINKER

$ riscv64-unknown-elf-ld -nostdlib -T memmap.ld  *.o -o larson.elf

Run with trace-symbol

SYMBOL TABLE, BINUTILS

$ riscv64-unknown-elf-nm larson.o
$ riscv64-unknown-elf-nm -n gpio.o
$ riscv64-unknown-elf-nm -n larson.elf
$ riscv64-unknown-elf-size larson.elf
$ riscv64-unknown-elf-size *.o

START SEQUENCE

view start.s cstart.c memmap.ld

BUILD ERRORS

cpp: wrong include path, bad directive
cc: semantic/synax, missing include
ld: undefined/multiple define


