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
 * `disassemble_is_valid_insn`
 *
 * Return number of bytes in encoding of instruction at addr if it is valid.
 * The address is valid if it lies within the text section and contents at
 * that address can be decoded as an instruction.
 *
 * @param addr      address of instruction
 * @return          2 or 4 if addr is valid text, 0 otherwise
 */
int disassemble_is_valid_insn(uintptr_t addr);

/*
 * `disassemble_decode_address`
 *
 * Return address of static buffer containing decoded contents at a given
 * address.  The format parameter indicates whether to decode contents
 * as instruction ('I'), word ('W'), half-word ('H'), or label ('L').
 */
const char *disassemble_decode_address(char format, void *address);

#endif
