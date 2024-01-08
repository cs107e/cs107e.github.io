Demo of cross-development toolchain

Assembly a one instruction program:

    addw    a0,a1,a2


# assemble add.s to make an 'object file' add.o
riscv64-unknown-elf-as add.s -o add.o

# number of bytes in add.o
ls -l add.o

# convert the object file to a binary file
riscv64-unknown-elf-objcopy add.o -O binary add.bin

# number of bytes in add.bin
ls -l add.bin

# "size" of add.o
riscv64-unknown-elf-size add.o

# print a hexdump of the binary
hexdump -C add.bin
