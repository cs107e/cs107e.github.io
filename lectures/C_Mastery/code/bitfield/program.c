#include "printf.h"
#include "strings.h"
#include "uart.h"

#include <stdint.h>

struct insn  {
    uint32_t opcode: 7;
    uint32_t reg_d:  5;
    uint32_t funct3: 3;
    uint32_t reg_s1: 5;
    uint32_t reg_s2: 5;
    uint32_t funct7: 7;
};

void change(struct insn *ptr) {
    ptr->reg_d = 10;
}

void main(void) {
    uart_init();

    printf("Disassemble instructions from main()\n");
    unsigned int *fn = (unsigned int *)main;
    for (int i = 0; i < 8; i++) {
        printf("%p:  %08x  %pI\n", &fn[i], fn[i], &fn[i]);
    }

    // what happens if you edit an instruction?
    struct insn *ptr = (struct insn *)fn;
    printf("\nWas:  %pI\n", ptr);
    change(ptr);
    printf("Now:  %pI\n", ptr);
}
