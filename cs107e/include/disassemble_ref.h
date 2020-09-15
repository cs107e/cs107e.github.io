#ifndef DISASSEMBLE_REF
#define DISASSEMBLE_REF
#include "disassemble.h"
bool ref_disassemble_insn(char *buf, size_t bufsize, unsigned int *addr);
void ref_disassemble_label(char *buf, size_t bufsize, unsigned int *addr);
unsigned int *ref_disassemble_name_to_addr(const char *name);
const char *ref_disassemble_addr_to_name(unsigned int *addr, int *p_offset);
bool ref_disassemble_is_valid_text(unsigned int *addr);
#endif
