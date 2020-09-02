#include "printf.h"
#include "uart.h"

static const char *board[] = {
    "O-------------------------------O ",
    "| oooooooooooooooooooo J8       | ",
    "| 1ooooooooooooooooooo          | ",
    "|                               | ",
    "|[RUN]   Pi Model A+ V1.1   +=====",
    "|         +-----+           | USB ",
    "| |D|     | SoC |           |     ",
    "| |S|     |     |           +=====",
    "| |I|     +-----+               | ",
    "|                   |C|         | ",
    "|                   |S|         | ",
    "|           |HDMI|  |I|  |A|    | ",
    "O-|pwr|-----|    |-------|V|----O "
};

static const char *header[20][2] = {
    { "3V3",     "5V" },
    { "GPIO2",   "5V" },
    { "GPIO3",   "GND" },
    { "GPIO4",   "GPIO14" },
    { "GND",     "GPIO15" },
    { "GPIO17",  "GPIO18" },
    { "GPIO27",  "GND" },
    { "GPIO22",  "GPIO23" },
    { "3V3",     "GPIO24" },
    { "GPIO10",  "GND" },
    { "GPIO9",   "GPIO25" },
    { "GPIO11",  "GPIO8" },
    { "GND",     "GPIO7" },
    { "GPIO0",   "GPIO1" },
    { "GPIO5",   "GND" },
    { "GPIO6",   "GPIO12" },
    { "GPIO13",  "GND" },
    { "GPIO19",  "GPIO16" },
    { "GPIO26",  "GPIO20" },
    { "GND",     "GPIO21" },
};


void main(void)
{
    uart_init();

    for (int i = 0; i < sizeof(board)/sizeof(*board); i++)
        printf("%s\n", board[i]);
    printf("\nJ8:\n");
    for (int i = 0; i < sizeof(header)/sizeof(*header); i++)
        printf(" %s\t   (%02d)\t(%02d)    %s\n", header[i][0], i*2+1, i*2+2, header[i][1]);
    
    uart_putchar(EOT);
}
