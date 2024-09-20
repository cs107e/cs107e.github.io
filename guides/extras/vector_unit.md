---
title: 'Extra: Guide to enable vector unit for SIMD operations'
---

*Written by Yifan Yang*

**You may find the XuanTie-OpenC906-UserManual useful for this guide. It will be referred to as the "user manual" or "manual" in this guide.**

The mango pi has a vector unit that can be used for SIMD operations. This guide provides an overview of the vector unit and how to use it in your code.

The vector unit in the mango pi cpu is disabled by default which means there is no power supplied to this area of the chip. To execute vector instructions, you need to enable the vector unit first or the program will crash whenever you touch the vector registers or use vector instructions. 

To enable the vector unit, we need to set the MXSTATUS CSR in Machine mode to enable the extension instruction set(which includes our vector instructions). Refering to the manual, the MXSTATUS CSR has a bit called THEADISAEE at bit 22(0-indexed). Setting this bit to 1 will enable the extension ISA.

After enabling the ISA, we can enable the vector unit by setting the VS bits in the MSTATUS CSR. The VS bits are at bit 23-24(0-indexed). Setting these bits to 01 will enable the vector unit.

Here is a sample code to enable the vector unit, please use it in start.s where our execution is in Machine mode.

```assembly
    lui     t0, 0x400        # set THEADISAEE bit in MXSTATUS
    csrs    mxstatus, t0     # (MXSTATUS is at 0x7C0)
    lui     t0, 0x800        # set VS bits in MSTATUS
    csrs    mstatus, t0      # (MSTATUS is at 0x300)
```

After enabling the vector unit, you can use the vector instructions in your code. The vector unit has 32 128-bit vector registers, v0-v31. You can use these registers to perform SIMD operations. The vector unit also has a set of vector instructions that can be used to perform operations on these registers. You can refer to the user manual for the list of vector instructions and their usage.

However, writing vector instructions directly in assembly can be difficult and error-prone. I recommend using the RVV extension for GCC to write vectorized code in C using rvv intrinsics.

To use thr RVV C intrinsics, we need a compatible compiler that can generate compatible vector instructions. Our MangoPi's D1-h chip supports an outdated RISC-V vector extension v0.7.1 which is neither binary compatible nor source compatible with the v1.0 RVV specification. We need to use a custom GCC compiler that supports the v0.7.1 vector extension. You can find the compiler in the `xuantie-gnu-toolchain` repository on GitHub, which is a riscv toolchain modified specifically for the MangoPi D1-h chip.

Clone the repository and build the toolchain following the instructions in the README. After building the toolchain, make sure you modify your Makefile or PATH to use the new compiler.

One last thing, you should give the compiler appropriate flags to specify the ARCH it is generating code for, so it knows to generate code for the v0.7.1 vector extension. You should use the `-march=rv64gcv0p7` flag when compiling your code.

After setting up the toolchain, you can use the RVV C intrinsics to write vectorized code in C. Here is an example of how to use the RVV C intrinsics to write a simple vectorized addition program.

```c
void main(void) 
{
    uart_init();
    printf("Testing vector unit\n");

    unsigned int a[4] = { 1, 2, 3, 4 };
    unsigned int b[4] = { 5, 6, 7, 8 };
    unsigned int c[4];

    size_t vl = vsetvl_e32m1 (4);
    printf("vl = %d\n", vl);
    vuint32m1_t v_a = vlwu_v_u32m1 (a, vl);
    vuint32m1_t v_b = vlwu_v_u32m1 (b, vl);
    vuint32m1_t v_c = vadd_vv_u32m1 (v_a, v_b, vl);
    vsw_v_u32m1 (c, v_c, vl);

    printf("c           = %d %d %d %d\n", c[0], c[1], c[2], c[3]);
    printf("Expected    = %d %d %d %d\n", a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
    printf("Test %s\n", (c[0] == a[0] + b[0] && c[1] == a[1] + b[1] && c[2] == a[2] + b[2] && c[3] == a[3] + b[3]) ? "PASSED" : "FAILED");
}
```

For guide on using the RVV C intrinsics, you can refer to the RVV v0.7.1 manual here: [RVV v0.7.1](https://occ-oss-prod.oss-cn-hangzhou.aliyuncs.com/resource//1663142187133/Xuantie+900+Series+RVV-0.7.1+Intrinsic+Manual.pdf)