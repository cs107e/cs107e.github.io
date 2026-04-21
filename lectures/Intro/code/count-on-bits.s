li a0, 67 # 0x43 or 0b01000011
li a1, 0
loop:
    andi a2, a0, 1
    add a1, a1, a2
    srli a0, a0, 1
    bne a0, zero, loop
