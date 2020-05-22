#include "console.h"
#include "echo.h"
#include "gpio.h"
#include "gpioextra.h"
#include "interrupts.h"
#include "keyboard.h"

#define BUTTON_GPIO GPIO_PIN21
#define NROWS 12
#define NCOLS 28

static volatile int gCount = 0;

static bool button_pressed(unsigned int pc)
{
    if (gpio_check_and_clear_event(BUTTON_GPIO)) {
        gCount++;
        return true;
    }
    return false;
}

static void update_screen()
{
    console_clear();
    console_printf("Count of button presses\n");
    for (int r = 0; r < NROWS*NCOLS/8; r++) {
        console_printf("%3d ", gCount);
    }
}

void main(void) 
{
    gpio_init();
    gpio_set_input(BUTTON_GPIO);
    gpio_enable_event_detection(BUTTON_GPIO, GPIO_DETECT_FALLING_EDGE);

    interrupts_init();
    interrupts_register_handler(INTERRUPTS_GPIO3, button_pressed);
    interrupts_global_enable();

    console_init(NROWS, NCOLS);
    console_printf("\n\nWaiting for button click...\n");

    int last_count = gCount;
    while (1) {
        if (last_count != gCount) {
            update_screen();
            last_count = gCount;
       }
    }
}
