Type of result of a binary operation between mixed types

gdb> ptype i8 -- prints type of an object
gdb> whatis i8 + u8 -- prints result type of an expression
gdb> source i8.gdb

% arm-none-eabi-gdb --commands=~/.gdbinit.arm  types.exe
gdb> break main
gdb> run
gdb> ptype i32
gdb> p i32+i32
gdb> whatis i32+i32
# promote i16 to i32 w/ sign extension
gdb> p i16+i32 
gdb> whatis i16+i32
gdb> p i8+i32
gdb> whatis i8+i32
gdb> p i32+i64
# promote i32 to i64 w/ sign extension
gdb> whatis i32+i64

# mixing signed and unsigned causes 
# the signed number to be converted to unsigned
gdb> p u32+i32
gdb> 0
gdb> whatis u32+i32
unsigned long
# YIPES
gdb> p u32
gdb> p i32
gdb> p u32<i32
gdb> 1 


