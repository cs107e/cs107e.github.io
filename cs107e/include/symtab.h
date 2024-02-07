#ifndef SYMTAB_H
#define SYMTAB_H

/*
 * Functions to operate on symtab data (if available):
 *    - find symbol for name
 *    - find symbol for address
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    const char *name;
    uintptr_t addr; // start address of symbol
    size_t size;    // size in bytes
} symbol_t;

/*
 * `symtab_find_name`
 *
 * Find symbol for `name`, store symbol information into
 * symbol_t reference parameter.
 *
 * @param name      name of function to search for
 * @return          true if found, false otherwise
 */
bool symtab_find_name(const char *name, symbol_t *p_symbol);

/*
 * `symtab_addr_to_name`
 *
 * Find symbol for `addr`, store symbol information into
 * symbol_t reference parameter.
 *
 * @param addr      address of instruction
 * @return          true if found, false otherwise
 */
bool symtab_find_addr(uintptr_t addr, symbol_t *p_symbol);

#endif
