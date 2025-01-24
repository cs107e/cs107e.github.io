# File blink.s
# ------------
# blink LED connected to GPIO PB0

    lui     a0,0x2000
    addi    a1,zero,1
    sw      a1,0x30(a0)

loop:

    sw      a1,0x40(a0)  # on
    lui     a2,0x3f00
    delay:
        addi    a2,a2,-1
        bne     a2,zero,delay

    sw      zero,0x40(a0) # off
    lui     a2,0x3f00
    delay2:
        addi    a2,a2,-1
        bne     a2,zero,delay2

    j       loop

