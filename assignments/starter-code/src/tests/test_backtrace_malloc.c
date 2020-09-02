#include "assert.h"
#include "backtrace.h"
#include "malloc.h"
#include "malloc_internal.h"
#include "nameless.h"
#include "printf.h"
#include "rand.h"
#include <stdint.h>
#include "strings.h"
#include "uart.h"

void main(void);

static void test_name_of(void)
{
    const char *name;

    name = name_of((uintptr_t)main);
    assert(strcmp(name, "main") == 0);
    name = name_of((uintptr_t)uart_init);
    assert(strcmp(name, "uart_init") == 0);
    name = name_of((uintptr_t)mystery); // function compiled without embedded name
    assert(strcmp(name, "???") == 0);
}

static void test_backtrace_simple(void)
{
    frame_t f[2];
    int frames_filled = backtrace(f, 2);

    assert(frames_filled == 2);
    assert(strcmp(f[0].name, "test_backtrace_simple") == 0);
    assert(f[0].resume_addr == (uintptr_t)test_backtrace_simple + f[0].resume_offset);
    assert(strcmp(f[1].name, "main") == 0);
    assert(f[1].resume_addr == (uintptr_t)main + f[1].resume_offset);
    printf("Here is a simple backtrace:\n");
    print_frames(f, frames_filled);
    printf("\n");
}

static int recursion_fun(int n)
{
    if (n == 0)
        return mystery();   // look in nameless.c
    else
        return 1 + recursion_fun(n-1);
}

static int test_backtrace_complex(int n)
{
    return recursion_fun(n);
}

static void test_heap_dump(void)
{
    heap_dump("Empty heap");

    char *p = malloc(24);
    heap_dump("After p = malloc(24)");

    free(p);
    heap_dump("After free(p)");

    p = malloc(16);
    heap_dump("After p = malloc(16)");

    p = realloc(p, 32);
    heap_dump("After p = realloc(p, 32)");

    free(p);
    heap_dump("After free(p)");
}

static void test_heap_simple(void)
{
    char *s;

    s = malloc(6);
    memcpy(s, "hello", 6);
    assert(strcmp(s, "hello") == 0);

    s = realloc(s, 12);
    strlcat(s, " world", 12);
    assert(strcmp(s, "hello world") == 0);
    free(s);
}

// array of dynamically-allocated strings, each
// string filled with repeated char, e.g. "A" , "BB" , "CCC"
// Examine each string, verify expected contents intact.
static void test_heap_multiple(void)
{
    int max_trials = 3;
    char *arr[max_trials*8];

    for (int ntrials = 1; ntrials <= max_trials; ntrials++) {
        int n = (ntrials*8);
        for (int i = 0; i < n; i++) {
            int num_repeats = i + 1;
            char *ptr = malloc(num_repeats + 1);
            assert(ptr != NULL);
            assert((uintptr_t)ptr % 8 == 0); // verify 8-byte alignment
            memset(ptr, 'A' - 1 + num_repeats, num_repeats);
            ptr[num_repeats] = '\0';
            arr[i] = ptr;
        }
        heap_dump("After all allocations");
        for (int i = n-1; i >= 0; i--) {
            int len = strlen(arr[i]);
            char first = arr[i][0], last = arr[i][len -1];
            assert(first == 'A' - 1 + len);  // verify payload contents
            assert(first == last);
            free(arr[i]);
        }
        heap_dump("After all frees");
    }
}

#define max(x, y) ((x) > (y) ? (x) : (y))

static void test_heap_recycle(int niter)
{
    extern int __bss_end__;
    void *heap_low = &__bss_end__, *heap_high = NULL;

    size_t max_size = 1024, total = 0;
    void *p = malloc(1);

    for (int i = 0; i < niter; i++) {
        size_t size = rand() % max_size;
        void *q = malloc(size);
        p = realloc(p, size);
        total += 2*size;
        void *higher = (char *)max(p, q) + size;
        heap_high = max(heap_high, higher);
        free(q);
    }
    free(p);
    size_t extent = (char *)heap_high - (char *)heap_low;
    size_t percent = total > extent ? (100*total)/extent : 0;
    printf("\nRecycling report for %d iterations:\n", niter);
    printf("Serviced requests totaling %d bytes, heap extent is %d bytes. Recycled %d%%\n", total, extent, percent);
}

void test_heap_redzones(void)
{
    // DO NOT ATTEMPT THIS TEST unless your heap has red zone protection!
    char *ptr;

    ptr = malloc(9);
    memset(ptr, 'a', 9); // write into payload
    free(ptr); // ptr is OK

    ptr = malloc(5);
    ptr[-1] = 0x45; // write before payload
    free(ptr);      // ptr is NOT ok

    ptr = malloc(12);
    ptr[13] = 0x45; // write after payload
    free(ptr);      // ptr is NOT ok
}


void main(void)
{
    uart_init();
    uart_putstring("Start execute main() in tests/test_backtrace_malloc.c\n");

    test_name_of();

    test_backtrace_simple();
    test_backtrace_simple(); // Again so you can see the main offset change!
    test_backtrace_complex(7);  // Slightly tricky backtrace

    test_heap_dump();

    test_heap_simple();
    test_heap_multiple();
    test_heap_recycle(20);
    
    //test_heap_redzones(); // DO NOT USE unless you implemented red zone protection

    uart_putstring("\nSuccessfully finished executing main() in tests/test_backtrace_malloc.c\n");
    uart_putchar(EOT);
}
