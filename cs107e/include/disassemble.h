#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

/*
 * Functions to operate on text section:
 *    - decode RISC-V instruction into a string
 *      (used by printf %pI format)
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * `disassemble_insn`
 *
 * Decodes instruction at location `addr` and writes string
 * form to `buf`, truncated to `bufsize`.
 *
 * @param buf       destination buffer where to write string
 * @param bufsize   size of destination buffer (output truncated to fit if needed)
 * @param addr      memory location of instruction to decode
 * @return          size in bytes of decoded instruction (4 or 2 if compressed)
 */
int disassemble_insn(char *buf, size_t bufsize, uintptr_t addr);

/*
 * `disassemble_is_valid_insn`
 *
 * Return num bytes in insn if valid. An insn address is valid if it lies within
 * the text section and contents at addr can be decoded as an instruction.
 *
 * @param addr      address of instruction
 * @return          2 or 4 if addr is valid text, 0 otherwise
 */
int disassemble_is_valid_insn(uintptr_t addr);

#endif
