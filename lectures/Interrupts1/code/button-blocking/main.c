#include "gpio.h"
#include "gpioextra.h"
#include "console.h"

#define BUTTON_GPIO GPIO_PIN21
#define NROWS 5
#define NCOLS 20

static int count = 0;

static void wait_for_button(void)
{
    while (gpio_read(BUTTON_GPIO) == 0) ;
    while (gpio_read(BUTTON_GPIO) == 1) ;
    count++;
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

    console_init(NROWS, NCOLS);
    console_printf("Click button...");

    while (1) {
        wait_for_button();
        update_screen();
    }
}
