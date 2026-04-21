# File: blink.s
# -------------
# blink LED connected to GPIO PB0

_start:
    lui     a0,0x2000       # a0 holds base addr PB group = 0x2000000
    addi    a1,zero,1
    sw      a1,0x30(a0)     # config PB0 as output

loop:
    sw      a1,0x40(a0)     # turn on LED & wait for a bit
    lui     a2,0x3f00
delay:
    addi    a2,a2,-1
    bne     a2,zero,delay   # count down to zero

    sw      zero,0x40(a0)   # turn off LED & wait for a bit
    lui     a2,0x3f00
delay2:
    addi    a2,a2,-1
    bne     a2,zero,delay2  # count down to zero

    j       loop            # back to top of outer loop
