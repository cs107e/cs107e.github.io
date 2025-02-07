#ifdef __riscv
#include "malloc.h"
#include "printf.h"
#include "uart.h"
#include "strings.h"
#else
#include "native.h"
#endif

struct {
    char arr[5];
    int count;
} data =  {
    "CS",
    0x107e,
};

static void overrun_data(int more) {
    printf("DATA: arr '%s'\ncount 0x%x\n", data.arr, data.count);

    printf("DATA: Writing %d bytes beyond... \n", more);
    memset(data.arr, 'M', sizeof(data.arr) + more);
    printf("DATA: arr '%s'\ncount 0x%x\n", data.arr, data.count);
}

static void overrun_heap(int more) {
    int sz = 5;
    char *ptr1 = malloc(sz);
    char *ptr2 = malloc(sz);
    memset(ptr1, 0, sz);
    memset(ptr2, 0, sz);
    printf("HEAP: ptr1 '%s' ptr2 '%s'\n", ptr1, ptr2);

    printf("HEAP: Writing %d bytes beyond... \n", more);
    memset(ptr1, 'Z', sz + more);
    memset(ptr2, 'Y', sz + more);
    printf("HEAP: ptr1 '%s' ptr2 '%s'\n", ptr1, ptr2);
    free(ptr1);
    free(ptr2);
}

static void overrun_stack(int more) {
    char buf[5] = "abcd";
    printf("STACK: buf '%s'\n", buf);
    printf("STACK: Writing %d bytes beyond ... \n", more);
    memset(buf, 'Q', sizeof(buf) + more);
    printf("STACK: buf '%s'\n", buf);
}

int main(void) {
    uart_init();
    printf("\n\n Starting %s \n\n", __FILE__);

    // overrun_data(10);
    // overrun_heap(100);
    // overrun_stack(100);
    printf("\n\n Completed main() of %s \n", __FILE__);
    return 0;
}
