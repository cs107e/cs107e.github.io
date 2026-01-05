---
permalink: /resources/
title: Informational resources
toc: true
---

Here are links to various external resources with information about the Mango Pi, RISC-V assembly, C, and more.

## Mango Pi

- Manufacturer's site Mango Pi MQ-Pro <https://widora.cn/mqpro>
- Mango Pi [schematic/pinout](/readings/mq-prov14-sch.pdf)
- Tour the Mango Pi board using its [interactive BOM](/readings/mq-pro-v12-ibom.html)
- The Mango Pi uses an Allwinner D1-H chip. The peripherals are documented in the [Allwinner D1-H User Manual](/readings/d1-h_user_manual_v1.0.pdf)
    - The user manual is 1400 pages of nitty-gritty goodness (addresses, layouts, behaviors). It's a tome for sure, but keep this one bookmarked, you'll be looking at it often.
- The D1 has a single RV64GCV core XuanTie C906 made by T-Head Semiconductors.
    - Processor overview <https://www.riscfive.com/2023/03/09/t-head-xuantie-c906-risc-v/>
    - Full documentation [XuanTie-Openc906-UserManual](/readings/XuanTie-OpenC906-UserManual.pdf)

## RISC-V architecture

- _The Risc-V Reader: An Open Architecture Atlas_, Patterson & Waterman <https://www.amazon.com/dp/0999249118/>
    - Amazingly succinct yet thorough coverage of the architecture, with backstory on how the design came to be.
- Riscv.org website <https://riscv.org/technical/specifications/>
    - [RISC-V specification Vol. 1](/readings/riscv-spec-20191213.pdf) and [Vol. 2](/readings/riscv-privileged-20190608-1.pdf)
- _Computer Organization and Design: The Hardware Software Interface_ <https://www.amazon.com/dp/0128203315/>
    - Patterson and Hennessy's classic text, RISC-V edition
- _An Overview of the RISC-V Instruction Set Architecture_, Harry Porter <https://web.cecs.pdx.edu/~harry/riscv/>
- Andrew Waterman's 2016 PhD thesis _"Design of the RISC-V Instruction Set Architecuture"_  <https://people.eecs.berkeley.edu/~krste/papers/EECS-2016-1.pdf>

## RISC-V assembly

- ChibiAliens has a series of video tutorials on RISC-V <https://www.chibialiens.com/riscv/>
- RISC-V Assembly Programmer's Manual <https://github.com/riscv-non-isa/riscv-asm-manual/blob/master/riscv-asm.md>
 - _RISC-V Assembly for Beginners_, blog post from Eric Engheim <https://medium.com/swlh/risc-v-assembly-for-beginners-387c6cd02c49>
- Eric Engheim's one-page cheatsheet <http://blog.translusion.com/images/posts/RISC-V-cheatsheet-RV32I-4-3.pdf>
- See our [Demos page](/demos/) for simulators/tools.

## Bare metal programming

- David Welch has extensive github repository of bare metal programming examples for a wide variety of architectures. Check out  <https://github.com/dwelch67/lichee-rv-samples>, the lichee RV is using the same AllWinner D1 chip as Mango Pi.
- Embedded Systems Developer RISC-V Blog <https://five-embeddev.com/quickref/> (Phil Mulholland)

## C language

- Nick Parlante's [EssentialC](http://cslibrary.stanford.edu/101)
- _The C Programming Language, 2nd Ed._, B. Kernighan and D. Ritchie.
A digital copy of K&R is available to Stanford students via 
[Safari Books Online](https://proquest-safaribooksonline-com.stanford.idm.oclc.org/9780133086249)

## Developer tools

- _The Definitive Guide to GCC, 2nd Ed._, William von Hagen, Apress, 2006
[pdf](http://sensperiodit.files.wordpress.com/2011/04/hagen-the-definitive-guide-to-gcc-2e-apress-2006.pdf)
