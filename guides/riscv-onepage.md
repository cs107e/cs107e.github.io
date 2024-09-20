---
title: One page of RISC-V
attribution: Prepared by Julie Zelenski
---

<style>

    .insn td:nth-child(3), .reg td:nth-child(1), .reg td:nth-child(2), .pseudo td:nth-child(2), .pseudo td:nth-child(3) {
        font-weight: 500;
        font-family: Inconsolata, Consolas, Menlo, monospace;
        color: #B52741;
    }
    .insn td:nth-child(1), .pseudo td:nth-child(1) {
        font-weight: 700;
    .insn {
        margin-bottom: 2rem;
    }
    thead {
        font-weight: 700;
    }
</style>


Below is a brief summary of common RISC-V instructions. See the [Resources](/resources#riscv-assembly) page for links to more complete tutorials and readings about RISC-V assembly. We particularly like Eric Engheim's RISC-V cheatsheet:

<a href="http://blog.translusion.com/images/posts/RISC-V-cheatsheet-RV32I-4-3.pdf">
![](../images/riscv-cheatsheet.png){: .border .mx-auto .w-25}
</a>

## Registers
There are 32 registers indexed from `x0` to `x31`. Each register has an alias that aids as mnemonic for the register's intended use. We will typically refer to registers by alias name.

| Register |  Alias | Purpose
|:--------|:------|:------|
| x0  | zero | fixed zero value
| x1  | ra | return address
| x2  | sp | stack pointer
| x3  | gp | global pointer
| x4  | tp | thread pointer
| x5  | t0 | temporary
| x6  | t1 | temporary
| x7  | t2 | temporary
| x8  | s0/fp | saved by callee, frame pointer
| x9  | s1 | saved by callee
| x10 | a0 | function argument 1, function return value
| x11 | a1 | function argument 2
| x12 | a2 | function argument 3
| x13 | a3 | function argument 4
| x14 | a4 | function argument 5
| x15 | a5 | function argument 6
| x16 | a6 | function argument 7
| x17 | a7 | function argument 8
| x18 | s2 | saved by callee
| x19 | s3 | saved by callee
| x20 | s4 | saved by callee
| x21 | s5 | saved by callee
| x22 | s6 | saved by callee
| x23 | s7 | saved by callee
| x24 | s8 | saved by callee
| x25 | s9 | saved by callee
| x26 | s10 | saved by callee
| x27 | s11 | saved by callee
| x28 | t3 | temporary
| x29 | t4 | temporary
| x30 | t5 | temporary
| x31 | t6 | temporary
{: .table .table-sm .table-striped .reg }

## Common instructions

`rd`: destination register, `rs`: source register, `imm`: immediate value (constant)

| Opcode | Instruction |  Syntax | Notes |
|:--------|:------|:--------|
||__ALU instructions (R-type)__
| ADD | Add     | ADD rd,rs1,rs2
| SUB | Subtract | SUB rd,rs1,rs2
| AND | Bitwise and |AND rd,rs1,rs2
| OR | Bitwise or | OR rd,rs1,rs2
| XOR | Bitwise exclusive or | XOR rd,rs1,rs2
| SLL | Shift left logical | SLL rd,rs1,rs2
| SRL | Shift right logical | SRL rd,rs1,rs2
| SRA | Shift right arithmetic | SRA rd,rs1,rs2
|| __ALU (I-type)__ |
| ADDI | Add immediate     | ADDI rd,rs,imm12
| ANDI | And immediate | ANDI rd,rs,imm12
| ORI | Or immediate | ORI rd,rs,imm12
| XORI | Xor immediate| XORI rd,rs,imm12
| SLLI | Shift left logical immediate | SLLI rd,rs,imm12
| SRLI | Shift right logical immediate | SRLI rd,rs,imm12
| SRAI | Shift right arithmetic immediate | SRAI rd,rs,imm12
||__Branch instructions__
|BEQ| Branch equal  | BEQ rs1,rs2,imm12 | target = PC + imm12 (e.g. PC-relative)
|BNE| Branch not equal  | BNE rs1,rs2,imm12
|BGE| Branch greater or equal  | BGE rs1,rs2,imm12
|BLT| Branch less than  | BLT rs1,rs2,imm12
|JAL| Jump and link | JAL rd,imm20
|JALR| Jump and link register |  JALR rd,imm12(rs)
||__Load/store instructions__ | |   suffixes D=double-word, H=half-word B=byte
| LW | Load word | LW rd,imm12(rs)
| SW | Store word | SW rs,imm12(rd)
{: .table .table-sm .table-striped .insn }

## Pseudo instructions
These are accepted by the assembler as a convenience and converted into actual instruction(s).

| Mnemonic | Example | Translation | Notes |
|:--------|:------|:--------|:--------|
|LI |LI rd,imm32 | LUI rd,imm[31:12]    ADDI rd,rd,imm[11:0] | Load 32-bit imm (2 insns)
|MV|MV rd,rs | ADDI rd,zero,rs | Copy register
|NOT|NOT rd,rs | XORI rd,rs,-1 | Bitwise inverse
|NEG|NEG rd,rs | SUB rd,zero,rs | Arithmetic negation
|J|J imm20| JAL zero,imm20 | Unconditional jump
|CALL|CALL imm20| JALR ra,ra,imm20 | Function call
|RET|RET| JALR zero,0(ra) | Function return
{: .table .table-sm .table-striped .pseudo }

## More?
This page is just the highlights. Refer to the [RISC-V specification](/readings/riscv-spec-20191213.pdf) for information on:
- control and status registers `CSR`
- extensions (multiply/divide, compressed, atomic, vector)
- floating point

