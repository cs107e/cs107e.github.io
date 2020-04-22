#define GPIO_FSEL3  ((unsigned int *)0x2020000c)
#define GPIO_SET1   ((unsigned int *)0x20200020)
#define GPIO_CLR1   ((unsigned int *)0x2020002c)

// Red power LED (on Pi board) is GPIO 35.
#define ABORT_OUTPUT (1 << (3*5))
#define ABORT_BIT    (1 << (35-32))

#define DELAY 0x100000

// abort goes into an infinite loop that flashes the red power LED.
void abort(void) {
    // First, configure GPIO 35 function to be output.
    // This assignment wipes functions for other pins in this register
    // (GPIO 30-39), but that's okay, because this is a dead-end routine.
    *GPIO_FSEL3 = ABORT_OUTPUT;
    while (1) {
        *GPIO_SET1 = ABORT_BIT;
        // These delay loops have volatile counters to prevent being
        // optimized out.
        for (volatile int i = 0; i < DELAY; i++) ;
        *GPIO_CLR1 = ABORT_BIT;
        for (volatile int i = 0; i < DELAY; i++) ;
    }
}
