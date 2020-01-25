Example showing how two programs, main and tricky, are linked.

% make clean
% make tricky.o
% arm-none-eabi-nm -S tricky.o
00000004 00000004 C i
00000000 00000004 b j
00000000 00000004 D k
00000004 00000004 B l
00000000 00000004 R n
00000000 00000040 T tricky

# Note that the global uninitialized variable i is in common.

# Note that the static const variable m has been removed by the compiler.

% make blink.o
# print size of each symbol, sort numerically
% arm-none-eabi-nm -S -n main.o
         U i
         U k
         U tricky
00000000 00000038 T main

% make main.elf
% arm-none-eabi-nm -S -n main.elf 
00008000 T _start
0000800c t hang
00008010 00000038 T main
00008048 00000040 T tricky
00008088 00000058 T _cstart
000080e0 00000004 D k
000080e4 00000004 R n
000080e8 R __bss_start__
000080e8 00000004 b j
000080ec 00000004 B l
000080f0 00000004 B i
000080f8 B __bss_end__




