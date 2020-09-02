#define GPIO_FSEL3  ((volatile unsigned int *)0x2020000c)
#define GPIO_SET1   ((volatile unsigned int *)0x20200020)
#define GPIO_CLR1   ((volatile unsigned int *)0x2020002c)

// Red power LED (on Pi board) is GPIO 35.
#define ABORT_OUTPUT (1 << ((35-30)*3))
#define ABORT_BIT    (1 << (35-32))

#define DELAY 0x100000

void abort(void) {

    // Configure GPIO 35 function to be output.
    // Below statement wipes functions for other pins in same register
    // (GPIO 30-39), but that's okay, because this is a dead-end routine.
    *GPIO_FSEL3 = ABORT_OUTPUT;

    // Infinite loop that flashes the red power LED
    while (1) {
        *GPIO_SET1 = ABORT_BIT;
        // Delay loop has volatile counter to prevent being optimized out
        for (volatile int i = 0; i < DELAY; i++) ;
        *GPIO_CLR1 = ABORT_BIT;
        for (volatile int i = 0; i < DELAY; i++) ;
    }
}
