#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

/*
 * Functions to operate on text section:
 *    - decode RISC-V instruction into a string
 *      (used by printf %pI format)
 *    - translate address to symbolic name and vice versa
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>

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
int disassemble_insn(char *buf, size_t bufsize, const void *addr);

/*
 * `disassemble_label`
 *
 * Determines label for instruction at address and writes label
 * to `buf`, truncated to  `bufsize`. Label is in the form
 * "<function+offset>" if function name available, otherwise "<>"
 *
 * @param buf       destination buffer where to write label
 * @param bufsize   size of destination buffer (output truncated to fit if needed)
 * @param addr      memory location of instruction
 */
void disassemble_label(char *buf, size_t bufsize, const void *addr);

/*
 * `disassemble_name_to_addr`
 *
 * Searches text section to find a function with embedded name that
 * matches `name`.
 *
 * @param name      name of function to search for
 * @return          address of function's first instruction, NULL if not found
 */
const void *disassemble_name_to_addr(const char *name);

/*
 * `disassemble_addr_to_name`
 *
 * Searches backwards from `addr` to find embedded name for the
 * function containing instruction at `addr`. If `p_offset` is non-NULL,
 *`*p_offset` is assigned the count of bytes bewteen `addr` and
 * the function's first instruction.
 *
 * @param addr      address of instruction
 * @param p_offset  output parameter, *p_offset = number of bytes into function
 * @return          function's name, NULL if not found
 */
const char *disassemble_addr_to_name(const void *addr, int *p_offset);

/*
 * `disassemble_is_valid_insn`
 *
 * Return num bytes in insn if valid. An insn ddress is valid if it lies within
 * the text section and contents at addr can be decoded as an instruction.
 *
 * @param addr      address of instruction
 * @return          2 or 4 if addr is valid text, 0 otherwise
 */
int disassemble_is_valid_insn(const void *addr);

#endif
