#ifdef __riscv
#include "malloc.h"
#include "printf.h"
#include "uart.h"
#include "strings.h"
#else
#include "native.h"
#endif


void bad_deref(void *ptr) {
    printf("BAD DEREF: attempt to read from address %p\n", ptr);
    int num = *(int *)ptr;
    printf("Ok? *(%p) = %x\n", ptr, num);
}

int infinite_recur(int arg) {
    if (arg == 0) return 0;
    if (arg % 10000 == 0) printf("recur(%d)...\n", arg);
    return arg + infinite_recur(arg+1);
}

void corrupt_string_literal(const char *str) {
    printf("\nREAD-ONLY: attempt to change string constant %s\n", str);
    char *casted = (char *)str;
    casted[0] = 'J';
    casted[1] = 'Z';
}

int g_nums[2] = {0x107e, 5};
char g_buf[8] = "abcdefg";

void overrun_data(int more) {
    printf("\nDATA: g_buf '%s' g_nums[0] 0x%x\n", g_buf, g_nums[0]);

    printf("DATA: Writing %d bytes beyond... \n", more);
    memset(g_buf, '$', sizeof(g_buf) + more);
    printf("DATA: g_buf '%s' g_nums[0] 0x%x\n", g_buf, g_nums[0]);
}

void overrun_heap(int more) {
    int sz = 5;
    char *ptr1 = malloc(sz);
    char *ptr2 = malloc(sz);
    printf("how long is ptr 2? %ld\n", strlen(ptr2+more*2));

    ptr1[0] = 'A'; ptr1[1] = '\0';
    ptr2[0] = 'B'; ptr2[1] = '\0';
    printf("\nHEAP: ptr1 '%s' ptr2 '%s'\n", ptr1, ptr2);

    printf("HEAP: Writing %d bytes beyond... \n", more);
    memset(ptr2, 'Y', sz + more);
    ptr2[sz+more] = '\0';
    memset(ptr1, 'Z', sz + more);
    ptr1[sz+more] = '\0';
    printf("HEAP: ptr1 '%s' ptr2 '%s'\n", ptr1, ptr2);
    free(ptr1);
    free(ptr2);
}

void overrun_stack(int more) {
    char buf[5] = "abcd";
    printf("\nSTACK: buf '%s' \n", buf);
    printf("STACK: Writing %d bytes beyond ... \n", more);
    memset(buf, 'R', sizeof(buf) + more);
    printf("STACK: buf '%s'\n", buf);
}

int main(void) {
    uart_init();
    printf("\n\n Starting %s \n\n", __FILE__);

    //bad_deref(NULL);
    //infinite_recur(10);
    //corrupt_string_literal("Hello");
    printf("Print other string constant: %s\n", "Hello");
    //overrun_data(6);
    //overrun_heap(500);
    //overrun_stack(32);

    printf("\n\n Completed main() of %s \n", __FILE__);
    return 0;
}
