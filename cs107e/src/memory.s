.globl PUT32
PUT32:
    sw  a1,0(a0)
    ret

.globl GET32
GET32:
    lw  a0,0(a0)
    ret

.globl PUT16
PUT16:
    sh  a1,0(a0)
    ret

.globl GET16
GET16:
    lhu a0,0(a0)
    ret

.globl PUT8
PUT8:
    sb  a1,0(a0)
    ret

.globl GET8
GET8:
    lbu a0,0(a0)
    ret
