#include "gpio.h"
extern void main(void);

void _cstart(void) {
    extern char __bss_start, __bss_end;

    for (char *cur = &__bss_start; cur < &__bss_end; cur++) {
        *cur = 0;
    }
    gpio_set_output(GPIO_PD18);
    gpio_write(GPIO_PD18, 1);   // turn on blue ACT led before main()
    main();
    gpio_write(GPIO_PD18, 0);   // turn off after main() completes
}
