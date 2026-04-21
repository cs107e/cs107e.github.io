#ifndef SYMTAB_H
#define SYMTAB_H

/*
 * Functions to operate on symbol table (if available):
 *    - label for address
 *    - find symbol for name
 *    - find symbol for address
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * `symtab_label_for_addr`
 *
 * Finds function symbol for instruction at address and writes label
 * to `buf`, truncated to `bufsize`. If symbol names are available,
 * label is of form "<fn_name+offset>". Without symbol names,
 * label is "<.text+offset>" for an address within the text
 * section and "<>" otherwise.
 *
 * @param buf       destination buffer where to write label
 * @param bufsize   size of destination buffer (label truncated to fit if needed)
 * @param addr      addr to provide label for
 */
void symtab_label_for_addr(char *buf, size_t bufsize, uintptr_t addr);


typedef struct {
    const char *name;   // symbol name
    uintptr_t addr;     // symbol start address
    size_t size;        // symbol size in bytes
} symbol_t;

/*
 * `symtab_symbol_for_name`
 *
 * Search for a function symbol with the specified `name`.
 * If symbol with name is found, the symbol info is written to
 * the parameter `*p_symbol` and function returns true.
 * If no match found, function returns false and *p_symbol is
 * unchanged.
 *
 * @param name      name of function to search for
 * @param p_symbol  pointer to where to store symbol data
 * @return          true if found, false otherwise
 */
bool symtab_symbol_for_name(const char *name, symbol_t *p_symbol);

/*
 * `symtab_symbol_for_addr`
 *
 * Search for a function symbol that contains the specified `addr`.
 * If symbol containing addr is found, the symbol info is written to
 * the parameter `*p_symbol` and function returns true.
 * If no match found, function returns false and *p_symbol is
 * unchanged.
 *
 * @param addr      address to search for
 * @param p_symbol  pointer to where to store symbol data
 * @return          true if found, false otherwise
 */
bool symtab_symbol_for_addr(uintptr_t addr, symbol_t *p_symbol);

#endif
