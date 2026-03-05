# Julie Zelenski, Feb 2024
# Sample assembly to drive a Neopixel/WS8212
#
# if zero bit, hold data high 200-500 nanosecond
# if one bit, hold data high 550-850 nanosecond (2x width of zero bit)
# end with hold data low >= 800 nanosecond

.equ NS_PER_LOOP_CYCLE, 5  # add (1) + bne (4)
.equ CYCLES_PER_ZERO_BIT, 350/NS_PER_LOOP_CYCLE
.equ CYCLES_OFF, 800/NS_PER_LOOP_CYCLE

# # send_bit(a0 = addr of gpio data reg, a1 = data bit to flip, a2 = zero/one)
.global send_bit
    send_bit:
        lw a4,0(a0)
        xor a4,a4,a1
        sw a4,0(a0)   # gpio = on
        li a3,CYCLES_PER_ZERO_BIT
        sll a3,a3,a2   # hold one bit twice as long as zero bit
        addi a3,a3,-1
        bne a3,zero,.-4
        not a1,a1
        and a4,a4,a1
        sw a4,0(a0) # gpio = off
        li a3,CYCLES_OFF
        addi a3,a3,-1
        bne a3,zero,.-4
        ret

