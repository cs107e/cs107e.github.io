@ Julie Zelenski, Winter 2019
@ Sample assembly to drive a Neopixel/WS8212
@
@ Datasheet https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
@ https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/
@ Neopixel timing is fussy, but above v. helpful blog post
@ makes good sense of it. The only truly critical step is keeping
@ width of zero bit short enough to not be interpreted as one bit.
@
@ if zero bit, hold data high 200-500 nanosecond
@ if one bit, hold data high 550-850 nanosecond
@ bring data low afterwards for >= 800 nanosecond

.equ NS_PER_INSTRUCTION, 50 
.equ NS_PER_LOOP_CYCLE, 100  @ 2 instructions in loop
.equ CYCLES_PER_ZERO_BIT, 350/NS_PER_LOOP_CYCLE
.equ CYCLES_PER_ONE_BIT, 700/NS_PER_LOOP_CYCLE
.equ CYCLES_OFF, 800/NS_PER_LOOP_CYCLE

@ send_bit(r0 = gpio_pin, r1 = zero/one)
.global send_bit
    send_bit:
        ldr r2, SET0
        mov r3, #1
        lsl r0, r3, r0   @ r0 is which gpio bit to set/clear
        str r0, [r2]
        tst r1, r1
        movne r1, #CYCLES_PER_ONE_BIT
        moveq r1, #CYCLES_PER_ZERO_BIT
        subs r1, #1
        bne .-4
        ldr r2, CLR0
        str r0, [r2]
        mov r1, #CYCLES_OFF
        subs r1, #1
        bne .-4
        bx lr

SET0: .word 0x2020001c
CLR0: .word 0x20200028
