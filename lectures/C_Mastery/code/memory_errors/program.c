#ifdef __riscv
#include "malloc.h"
#include "printf.h"
#include "uart.h"
#include "strings.h"
#else
#include "native.h"
#endif

static void bad_deref(void *ptr) {
    printf("BAD DEREF: attempt to read from address %p\n", ptr);
    int num = *(int *)ptr;
    printf("Ok? *(%p) = %x\n", ptr, num);
}

static int infinite_recur(int arg) {
    if (arg == 0) return 0;
    if (arg % 10000 == 0) printf("recur(%d)...\n", arg);
    return arg + infinite_recur(arg+1);
}

static void corrupt_string_literal(const char *str) {
    printf("\nREAD-ONLY: attempt to change string constant \"%s\"\n", str);
    char *casted = (char *)str;
    casted[0] = 'J';
    casted[1] = 'Z';
}

static void overrun_heap(int more) {
    int sz = 5;
    char *heap_ptr1 = malloc(sz);
    char *heap_ptr2 = malloc(sz);

    memset(heap_ptr1, 'A', sz);
    heap_ptr1[sz-1] = '\0';
    memset(heap_ptr2, 'B', sz);
    heap_ptr2[sz-1] = '\0';
    printf("\nHEAP: heap_ptr1 %p [%s] heap_ptr2 %p [%s]\n", heap_ptr1, heap_ptr1, heap_ptr2, heap_ptr2);

    printf("HEAP: Writing %d bytes beyond heap_ptr1/heap_ptr2 size ... \n", more);
    memset(heap_ptr2, 'A', sz + more);
    heap_ptr2[sz+more] = '\0';
    memset(heap_ptr1, 'B', sz + more);
    heap_ptr1[sz+more] = '\0';
    printf("HEAP: heap_ptr1 %p [%s] heap_ptr2 %p [%s]\n", heap_ptr1, heap_ptr1, heap_ptr2, heap_ptr2);
    free(heap_ptr1);
    free(heap_ptr2);
}

static void overrun_stack(int more) {
    char stackbuf[5] = "abcd";
    printf("\nSTACK: stackbuf %p [%s] \n", stackbuf, stackbuf);
    printf("STACK: Writing %d bytes beyond bufsize ... \n", more);
    memset(stackbuf, 'R', sizeof(stackbuf) + more);
    printf("STACK: stackbuf %p [%s]\n", stackbuf, stackbuf);
}

int main(void) {
    uart_init();
    printf("\n\n Starting %s \n\n", __FILE__);

    //bad_deref(NULL);

    //infinite_recur(10);

    //corrupt_string_literal("Hello");
    //printf("Print string constant: %s\n", "Hello");

    overrun_heap(12);
    overrun_stack(20);

    printf("\n\n Completed main() of %s \n", __FILE__);
    return 0;
}
