#ifndef MEMORY_H
#define MEMORY_H
/*
 * Macros for explicit memory reads and writes.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Date: Jan 1, 2024
 */

#include <stdint.h>

uint32_t GET32(uintptr_t addr);
void PUT32(uintptr_t addr, uint32_t val);

uint16_t GET16(uintptr_t addr);
void PUT16(uintptr_t addr, uint16_t val);

uint8_t GET8(uintptr_t addr);
void PUT8(uintptr_t addr, uint8_t val);

#endif
