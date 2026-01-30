#include "uart.h"
#include "printf.h"

                // uninitialized
int gNum;
static int sgNum;

                // initialized
int iNum = 0x1;
static int siNum = 0x22;

                // const
const int cNum = 0x333;
static const int scNum = 0x4444;


static void show_var(const int *p, const char *name) {
    printf("Variable %8s at address %p value 0x%x\n", name, p, *p);
}

void main(void) {
    int local[4];

    uart_init();

    show_var(&gNum, "gNum");
    show_var(&sgNum, "sgNum");
    show_var(&iNum, "iNum");
    show_var(&siNum, "siNum");
    show_var(&cNum, "cNum");
    show_var(&scNum, "scNum");
    show_var(&local[0], "local[0]");
    show_var(&local[1], "local[1]");
}
