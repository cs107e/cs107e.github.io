/* File: buggy.c
 * -------------
 * This buggy program uses local variables that are not properly
 * initialized. The contents will be garbage/unpredictable.
 * Run this program on the Pi, and then again in gdb under
 * simulation.  Are the garbage contents the same?  Do you get
 * the same results on subsequent runs? 
 */
#include "printf.h"
#include "uart.h"


void stack_array(void)
{
    char array[32];         // not initialized before read

    printf("array[] = {");
    for (int i = 0; i < 5; i++) {
        printf(" 0x%x ", array[i]);
        array[i]++;     // increment starting from garbage
    }
    printf("}\n");
}

int local(int n)
{
    int result;     // not initialized
    for (int i = 0; i < n; i++)
        result++;   // increment n times starting from garbage
    return result;
}

void main(void)
{
    uart_init();

    printf("1st call stack_array (no initialization): ");
    stack_array();
    printf("2nd call stack_array (contents leftover): ");
    stack_array();
    int result = local(1);
    printf("result from local() = %d\n", result);

    uart_putchar(EOT);
}
