---
title: One page of ARM
---

<style>
    .arm td:nth-child(3), .reg td:nth-child(1) {
        font-weight: 500;
        font-family: Inconsolata, Consolas, Menlo, monospace;
        color: #B52741;
    }
    .arm td:nth-child(1)  {
        font-weight: 700;
    }
    .arm {
        margin-bottom: 2rem;
    }
    thead {
        font-weight: 700;
    }
</style>


Very brief summary of ARM instructions. Our [Resources](/resources#arm-assembly) page has links to more complete tutorials and readings about ARM assembly.

## Registers
There are 16 registers `r0` to `r15`. Most are general-use, a few are dedicated for particular purpose.  Registers designed as "scratch" (r0-3, r12) are callee-owned, the remainder are caller-owner.

| Register |  Purpose
|:--------|:------|
| r0  | function param 1, function return, scratch
| r1  | param 2, scratch |
| r2  | param 3, scratch |
| r3  | param 4, scratch |
| r4 - r10 | | 
| r11  | Frame pointer (`fp`) |
| r12  | Intraprocedural scratch  (`ip`) |
| r13  | Stack pointer (`sp`) |
| r14  | Link register (`lr`) |
| r15  | Program counter (`pc`) |
{: .table .table-sm .table-striped .reg }

## Common instructions

| Opcode | Instruction |  Syntax | Notes |
|:--------|:------|:--------|
||||_Data Processing instructions_ |
|:--------|:-------|:--------|
| ADD | Add     | ADD dest, op1, op2 |
| ADC | Add with Carry |ADC dest, op1, op2 |
| SUB | Subtract | SUB dest, op1, op2 |
| SBC | Subtract with Carry |SBC dest, op1, op2 |
| RSB |  Reverse Subtract |  RSB dest, op1, op2 |
| RSC | Reverse Subtract with Carry| RSC dest, op1, op2 |
| AND | Bitwise And |AND dest, op1, op2 |
| EOR | Bitwise Exclusive Or | EOR dest, op1, op2 |
| BIC | Bitwise Clear| BIC dest, op1, op2 |
| ORR | Bitwise Or |ORR dest, op1, op2 |
| CMP | Compare | CMP op1, op2 | op1 - op2 (next 4 insns set flags, discard result)
| CMN | Compare Negated |CMN op1, op2 | op1 + op2
| TST | Test | TST op1, op2 | op1 & op2 
| TEQ | Test Equals | TEQ op1, op2 | op1 ^ op2
| MOV | Move | MOV dest, op2 |
| MVN | Move Negated | MVN dest, op2 | bitwise inverse
|---------|
| | | |
| LDR | Load Register | LDR dest, [src] | 
| STR | Store Register | STR src, [dest] | Minor anomaly:  src register listed first
|B|Branch  | B target|
|BL| Branch and link | BL target |  Function call
|BX| Branch exchange | BX lr | Function return
{: .table .table-sm .table-striped .arm }


- __Op2 accepts an immediate value__. In the data processing instructions, `dest` and `op1` can only refer to a register, but `op2` accepts an immediate value encodable in a 8-bit value with 4-bit rotate. (Here is a neat exploration of what can be encoded under those constraints: <https://alisdair.mcdiarmid.org/arm-immediate-value-encoding>). For those of you pondering the need for both `SUB` and `RSB`, consider the impact of the asymmetry between `op1` and `op2` on the non-commutative subtract operation.
```
SUB r1, r1, #3
RSB r1, r1, #3
```


- __Use of barrel shifter on op2__.  Another distinction for `op2` of the data processing instructions is that the barrel shifter can be used to first shift or rotate this operand's value. For example, in the instruction below, `r3` is left shifted 5 positions before being added to `r2`.
```
ADD r1, r2, r3, LSL #5
```

    The available barrel shifter operations for `op2` are below. Here is nice explanation of the effect of each <http://www.davespace.co.uk/arm/introduction-to-arm/barrel-shifter.html>.

    | LSL | Logical Shift Left | op2, LSL #Imm | |
    | LSR | Logical Shift Right  | op2, LSR #Imm | |
    | ASR | Arithmetic Shift Right  | op2, ASR #Imm | |
    | ROR | Rotate Right   |op2, ROR #Imm | |
    | RRX | Rotate Right and Extend  | op2, RRX #Imm | |
    {: .table .table-sm .table-striped .arm}

    ARM does not have separate shift/rotate instructions; `LSL` `LSR` `ASR` are implemented as use of barrel shifter on op2.

    To read more about the ARM _Flexible second operand_, see <https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-instruction-set/about-the-instruction-descriptions/flexible-second-operand>

## Conditional execution
- __Condition codes__. The ARM has four condition codes  `Z` `N` `C` and `V` that each store a 1-bit state. The codes are set as a side effect of the Compare and Test instructions and can be optionally set by any data processing instruction by adding the `S` suffix (`XOR` -> `XORS`).

    |Z | Zero 
    |N | Negative
    |V | Overflow 
    |C | Carry
    {: .table .table-sm .table-striped}

- __Conditional execution__. Adding a condition suffix to instruction opcode, e.g. (`ADD` -> `ADDEQ`) causes the instruction to execute only if the specified condition is met, otherwise the instruction is skipped.  For an instruction specified with no suffix, `AL` is assumed. 

    | Condition | Opcode|  Execute if:  |
    |:--------|:-------|:--------|
    |EQ |Equal | Z
    |NE |Not equal | !Z
    |CS/HS |Carry set / unsigned higher or same | C
    |CC/LO | Carry clear / unsigned lower| !C
    |MI |Minus / negative | N
    |PL |Plus / positive or zero | !N
    |VS |Overflow | V
    |VC |No overflow | !V
    |HI |Unsigned higher| C and !Z
    |LS |Unsigned lower or same | !C or Z
    |GE |Signed greater than or equal |N == V
    |LT |Signed less than | N != V
    |GT |Signed greater than | !Z and (N == V)
    |LE |Signed less than or equal | Z or (N != V)
    |AL |Always (unconditional)| always
    {: .table .table-sm .table-striped .arm}

## More
This page is just the highlights -- there is much more! 
Refer to the [ARM manual](/readings/armisa.pdf) for information on:
- pre/post-index address modes
- `LDM` and `STM`, multiple load/store
- the `CPSR` special register and instructions `msr` and `mrs`
