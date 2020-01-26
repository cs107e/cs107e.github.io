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
 * `buf` is the destination of the instruction string
 * `bufsize` is the size of `buf`
 * `addr` is address of instruction in memory
 *
 * Decodes instruction at location `addr`, and writes string
 * form to `buf`, truncated (if necessary) to `bufsize`.
 * Returns true if able to decode instruction, false otherwise.
 */
bool disassemble_insn(char *buf, size_t bufsize, unsigned int *addr);

/*
 * `buf` is the destination of the label string
 * `bufsize` is the size of `buf`
 * `addr` is address of instruction in memory
 *
 * Writes label to `buf`, truncated (if necessary) to
 * `bufsize`. Label is in the form of "<function+offset>"
 * if function name found, otherwise just "<>"
 */
void disassemble_label(char *buf, size_t bufsize, unsigned int *addr);

/*
 * Searches text section to find a function with embedded name that
 * matches `name`.
 * If found, returns address of the function's first instruction;
 * otherwise returns NULL.
 */
unsigned int *disassemble_name_to_addr(const char *name);

/*
 * Searches backwards from `addr` to find embedded name for the
 * function containing instruction at `addr`. If `p_offset` is non-NULL,
 *`*p_offset` is assigned the count of bytes bewteen `addr` and
 * the function's first instruction.
 * Returns embedded name (if found), otherwise returns NULL.
 */
const char *disassemble_addr_to_name(unsigned int *addr, int *p_offset);

/*
 * Returns true if `addr` lies within the text section and contents at
 * addr can be decoded as an instruction; false otherwise.
 */
bool disassemble_is_valid_text(unsigned int *addr);

#endif