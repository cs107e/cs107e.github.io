.globl SETCPSR
SETCPSR:
    msr cpsr, r0
    bx lr

.globl SETSPSR
SETSPSR:
    msr spsr, r0
    bx lr

.globl GETCPSR
GETCPSR:
    mrs r0, cpsr
    bx lr

.globl GETSPSR
GETSPSR:
    mrs r0, spsr
    bx lr

