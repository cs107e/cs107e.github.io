#ifndef SYMTAB_H
#define SYMTAB_H

/*
 * Functions to operate on symbol table (if available):
 *    - find symbol for name
 *    - find symbol for address
 *    - label for address (used by printf %pL format)
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    const char *name;   // symbol name
    uintptr_t addr;     // symbol start address
    size_t size;        // symbol size in bytes
} symbol_t;

/*
 * `symtab_symbol_for_name`
 *
 * Search for a symbol matching `name`. If match found, store
 * symbol data into parameter `*p_symbol`.
 *
 * @param name      name of function to search for
 * @param p_symbol  pointer to where to store symbol data
 * @return          true if found, false otherwise
 */
bool symtab_symbol_for_name(const char *name, symbol_t *p_symbol);

/*
 * `symtab_symbol_for_addr`
 *
 * Search for a symbol that contains `addr`. If match found, store
 * symbol data into parameter `*p_symbol`.
 *
 * @param addr      address to search for
 * @param p_symbol  pointer to where to store symbol data
 * @return          true if found, false otherwise
 */
bool symtab_symbol_for_addr(uintptr_t addr, symbol_t *p_symbol);

/*
 * `symtab_label_for_addr`
 *
 * Determines label for instruction at address and write label
 * to `buf`, truncated to `bufsize`. If the symbol name is available,
 * label is of "<symbol_name+offset>". Without symbol names, the
 * basic label is "<.text+offset>" for an address within the text
 * section and "<>" otherwise.
 *
 * @param buf       destination buffer where to write label
 * @param bufsize   size of destination buffer (label truncated to fit if needed)
 * @param addr      addr to provide label for
 */
void symtab_label_for_addr(char *buf, size_t bufsize, uintptr_t addr);

#endif
