#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

/*
 * Various utility functions that operate on text section.
 *
 * Decode an ARM instruction into a string
 * (used by printf %pI format)
 *
 * Convert address to symbolic name and vice versa. (code
 * must be compiled -mpoke-function-name)
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Mon Feb 18 17:02:32 PST 2019
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * `disassemble_insn`
 *
 * Decodes instruction at location `addr`, and writes string
 * form to `buf`, truncated (if necessary) to `bufsize`.
 *
 * @param buf       destination buffer where to write output string
 * @param bufsize   size of destination buffer (output truncated to fit if needed)
 * @param addr      memory location of instruction to decode
 * @return          true if able to decode instruction, false otherwise
 */
bool disassemble_insn(char *buf, size_t bufsize, unsigned int *addr);

/*
 * `disassemble_label`
 *
 * Determines label for instruction at address and writes label
 * to `buf`, truncated (if necessary) to  `bufsize`. Label is
 * output in the form of "<function+offset>" if function name available,
 * otherwise just "<>"
 *
 * @param buf       destination buffer where to write label
 * @param bufsize   size of destination buffer (output truncated to fit if needed)
 * @param addr      memory location of instruction
 * @return          true if able to decode instruction, false otherwise
 */
void disassemble_label(char *buf, size_t bufsize, unsigned int *addr);

/*
 * `disassemble_name_to_addr`
 *
 * Searches text section to find a function with embedded name that
 * matches `name`.
 *
 * @param name      name of function to search for
 * @return          address of function's first instruction, NULL if not found
 */
unsigned int *disassemble_name_to_addr(const char *name);

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
const char *disassemble_addr_to_name(unsigned int *addr, int *p_offset);

/*
 * `disassemble_is_valid_text`
 *
 * An address is valid text if it lies within the text section and the
 * contents at addr can be decoded as an instruction.
 *
 * @param addr      address of instruction
 * @return          true if addr is valid text, false otherwise
 */
bool disassemble_is_valid_text(unsigned int *addr);

#endif