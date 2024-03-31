fibonacci:
        li      a0,10 # number of terms we want
        li      a1,0  # fibonacci number (1st term)
        li      a2,1  # fibonacci number (2nd term)
        j       L2
L3:
        add     a3,a1,a2 # nth term
        mv      a1,a2    # update first
        mv      a2,a3    # update second
        addi    a0, a0, -1 # decrement number of terms left
L2:
        bgt     a0, zero, L3 # branch if not finished
