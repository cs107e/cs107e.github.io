# File blink.s
# ------------
# blink LED connected to GPIO PB0

    lui     a0,0x2000
    addi    a1,zero,1
    sw      a1,0x30(a0)

loop:

    sw  a1,0x40(a0)  # turn on

    lui a2,0x3f00
delay_on:
        addi    a2,a2,-1
        bne     a2,zero,delay_on

    sw  zero,0x40(a0) # turn off

    lui a2,0x3f00
delay_off:
        addi    a2,a2,-1
        bne     a2,zero,delay_off

    j   loop

