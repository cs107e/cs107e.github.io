Demo of cross-development toolchain

Assembly a one instruction program:

    add    x3,x1,x2


# assemble add.s to make an 'object file' add.o
riscv64-unknown-elf-as add.s -o add.o

# number of bytes in add.o
ls -l add.o

# convert the object file to a binary file
riscv64-unknown-elf-objcopy add.o  add.bin -O binary

# number of bytes in add.bin
ls -l add.bin

# "size" of add.o
riscv64-unknown-elf-size add.o

# print a hexdump of the binary
hexdump -C add.bin
