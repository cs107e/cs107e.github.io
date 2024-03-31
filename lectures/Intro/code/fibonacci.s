fibonacci:
    addi    a0, zero, 10 # number of terms we want
    addi    a1, zero, 0  # fibonacci number (1st term)
    addi    a2, zero, 1  # fibonacci number (2nd term)
    j       L2
L3:
    add     a3, a1, a2 # nth term
    addi    a1, a2, 0  # update first
    addi    a2, a3, 0  # update second
    addi    a0, a0, -1 # decrement number of terms left
L2:
    bgt     a0, zero, L3 # branch if not finished

