#include "gpio.h"
#include "gpioextra.h"
#include "console.h"
#include "interrupts.h"

#define BUTTON_GPIO GPIO_PIN21
#define NROWS 5
#define NCOLS 20

static int count = 0;

static bool button_pressed(unsigned int pc)
{
    if (gpio_check_and_clear_event(BUTTON_GPIO)) {
        count++;
        return true;
    }
    return false;
}

static void update_screen(void)
{
    console_clear();
    for (int r = 0; r < NROWS*NCOLS/3; r++)
        console_printf("%d ", count);
}

void main(void) 
{
    gpio_init();
    gpio_set_input(BUTTON_GPIO);
    gpio_set_pullup(BUTTON_GPIO);
    gpio_enable_event_detection(BUTTON_GPIO, GPIO_DETECT_FALLING_EDGE);

    console_init(NROWS, NCOLS);
    console_printf("Click button...");

    interrupts_attach_handler(button_pressed);
    interrupts_enable_source(INTERRUPTS_GPIO3);
    interrupts_global_enable();

    int saved = count;
    while (1) {
        if (saved != count) {
            update_screen();
            saved = count;
       }
    }
}
