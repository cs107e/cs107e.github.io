# File on.s
# ---------
# turn on LED connected to GPIO PB0

# configure PB0 for output
# ------------------------
# Base address of GPIO peripheral is 0x02000000
# PB0 configured by config0 register of PB group
# PB config0 register is at offset 0x30 from GPIO base

    lui     a0,0x2000       # GPIO base address in a0
    addi    a1,zero,1       # constant 1 for output in a1
    sw      a1,0x30(a0)     # config PB0 as output

# set PB0 value to 1
# ------------------
# PB0 value is bit 0 in PB data register
# PB data register is at offset 0x40 from GPIO base

# a0 set to gpio base above, a1 has value 1
    sw      a1,0x40(a0)     # turn on PB0

# hang in infinite loop when done
loop:
    j loop
