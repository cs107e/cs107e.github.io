/* File: adc_demo.c
 * ---------------
 * SPI + MCP3008 adc test
 *
 * Breadboard configured for analog inputs:
 * 2-axis joystick channels 0/1
 * Photoresistor channel 6
 * Piezo channel 7
 */

#include "assert.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"
#include "strings.h"
#include "timer.h"

static const int MAX_VALUE = 1024;

#define STRINGIFY_IMPL(x) #x
#define AS_STRING(x) STRINGIFY_IMPL(x)
#define ANSI_ESC(n) "\e[" AS_STRING(n) "m"

#define RED     ANSI_ESC(41)
#define GREEN   ANSI_ESC(42)
#define YELLOW  ANSI_ESC(43)
#define BLUE    ANSI_ESC(44)
#define MAGENTA ANSI_ESC(45)
#define CYAN    ANSI_ESC(46)
#define WHITE   ANSI_ESC(47)
#define NORMAL  ANSI_ESC(0)


static void print_analog_scale(int val, const char *label, const char *color) {
    assert(val <= MAX_VALUE);
    int quanta = 30;
    int on_quantized = val/quanta;
    int off_quantized = MAX_VALUE/quanta - on_quantized;
    char buf_on[on_quantized+1];
    char buf_off[off_quantized+1];
    memset(buf_off, ' ', sizeof(buf_off));
    memset(buf_on, '-', sizeof(buf_on));
    buf_on[sizeof(buf_on)-1] = '\0';
    buf_off[sizeof(buf_off)-1] = '\0';
    printf("%12s [%s%s%s%s]", label, color, buf_on, NORMAL, buf_off);
}

static void print_dashboard(int vert, int horiz, int light, int piezo){
    printf("\f\n     mcp3008 Readings  \n\n");
    print_analog_scale(vert, "Joy vert", CYAN);
    print_analog_scale(horiz, "Joy horiz", GREEN);
    printf("\n\n");
    print_analog_scale(light, "Light:", WHITE);
    printf("\n\n");
    print_analog_scale(piezo, "Piezo:", RED);
}

void main(void)  {
    gpio_init();
    uart_init();
    mcp3008_init();
    printf("Starting program %s\n\n", __FILE__);

    while (1) {
        int vert, horiz, light, piezo;
        vert = mcp3008_read(0);
        horiz = mcp3008_read(1);
        light = mcp3008_read(6);
        piezo = mcp3008_read(7);
        print_dashboard(vert, horiz, light, piezo);
        timer_delay_ms(100);
    }
}
