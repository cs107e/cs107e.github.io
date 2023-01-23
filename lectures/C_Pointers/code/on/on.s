    ldr r0, FSEL2
    mov r1, #1
    str r1, [r0]

    mov r1, #(1<<20)

    ldr r0, SET0
    str r1, [r0]

    loop: b loop

    FSEL2: .word 0x20200008
    SET0:  .word 0x2020001C
