# File blink.s
# ------------
# blink LED connected to GPIO PB0

    lui     a0,0x2000
    addi    a1,zero,1
    sw      a1,0x30(a0)     # config PB0 as output pin
loop:
    xori    a1,a1,1         # xor ^ 1 invert bit 0
    sw      a1,0x40(a0)     # update data register will flip on<->off
    lui     a2,0x3f00
delay:                      # busy loop to wait for a while
    addi    a2,a2,-1
    bne     a2,zero,delay
    j loop                  # repeat forever
