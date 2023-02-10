#include "printf.h"
#include "strings.h"
#include "uart.h"

#include <stdint.h>

struct insn  {
    uint32_t reg_op2:4;
    uint32_t one:1;
    uint32_t shift_op: 2;
    uint32_t shift: 5;
    uint32_t reg_dst:4;
    uint32_t reg_op1:4;
    uint32_t s:1;
    uint32_t opcode:4;
    uint32_t imm:1;
    uint32_t kind:2;
    uint32_t cond:4;
};

void change(struct insn *ptr) {
    ptr->reg_dst = 7;
}

void main(void) {

    printf("Disassemble instructions from function\n");
    unsigned int *fn = (unsigned int *)main;
    for (int i = 0; i < 8; i++) {
        printf("%p:  %x  %pI\n", &fn[i], fn[i], &fn[i]);
    }
    // what happens if you change instruction?
    struct insn *ptr = (struct insn *)fn;
    printf("\nWas:   %pI\n", ptr);
    change(ptr);
    printf("Now:   %pI\n", ptr);
    uart_putchar(EOT);
}
