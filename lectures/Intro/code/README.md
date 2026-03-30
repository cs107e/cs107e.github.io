# RISC-V instructions

Try pasting these instructions into the [Ripes visualizer](https://ripes.me/)

```
# li pseudoinstruction, look at executable code to see actual instructions
li a0,5
li a1,6

# rtype: 2 sources both regs
add a2,a1,a0
sub a2,a1,a0
and a2,a1,a0
or a2,a1,a0
xor a2,a1,a0

# itype: 1 source reg, 1 source 12-bit immediate (constant)

addi a2,a2,45
addi a2,a2,-1  # no subi
andi a2,a2,0xf
ori a2,a2,0xf
```