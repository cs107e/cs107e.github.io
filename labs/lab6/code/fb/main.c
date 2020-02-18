#include "fb.h"
#include "uart.h"

const int WIDTH = 640;
const int HEIGHT = 480;
const int DEPTH = 4;

void main(void)
{
    uart_init();

    fb_init(WIDTH, HEIGHT, DEPTH);

    uart_putchar(EOT);
}
