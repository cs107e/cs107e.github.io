.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl GET16
GET16:
    ldrh r0,[r0]
    bx lr

.globl GET8
GET8:
    ldrb r0,[r0]
    bx lr
