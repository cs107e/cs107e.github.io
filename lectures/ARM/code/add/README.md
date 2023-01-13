Demo of cross-development toolchain

Assembly a one instruction program:

    add r0, r1, r2


# assemble add.s to make an 'object file' add.o
arm-none-eabi-as add.s -o add.o

# number of bytes in add.o
ls -l add.o

# convert the object file to a binary file
arm-none-eabi-objcopy add.o -O binary add.bin

# number of bytes in add.bin
ls -l add.bin

# "size" of add.o
arm-none-eabi-size add.o

# print a hexdump of the binary
# hexdump add.bin
xxd -g 1 add.bin

