/* File: adc_demo.c
 * ----------------
 * SPI + MCP3008 adc test
 * juliez Mar 2025
 *
 * Breadboard configured for various analog inputs
 *  (potentiometer, 2-axis joystick, photoresistor, force sensor, analog hall
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

static const struct {
    const char *label;
    const char *color;
    int channel;
    int min; // will default to 0 if unset
} sensors[] = {
    { "Joy vert", CYAN, 0},
    { "Joy horiz", GREEN, 1},
    { "Pot", YELLOW, 7},
    { "Sound", RED, 2},
    { "Light", WHITE, 5},
    { "Magnet", MAGENTA, 6, MAX_VALUE/2},
    { "Force", BLUE, 4},
    {0}
};

static void print_analog_scale(int val, const char *label, const char *color, int min) {
    assert(val <= MAX_VALUE);
    int range = MAX_VALUE - min;
    int quanta = range/20;
    int on_quantized = (val-min)/quanta;
    int off_quantized = (MAX_VALUE-min)/quanta - on_quantized;
    char buf_on[on_quantized+1];
    char buf_off[off_quantized+1];
    memset(buf_off, ' ', sizeof(buf_off));
    memset(buf_on, '-', sizeof(buf_on));
    buf_on[sizeof(buf_on)-1] = '\0';
    buf_off[sizeof(buf_off)-1] = '\0';
    printf("%10s [%s%s%s%s]", label, color, buf_on, NORMAL, buf_off);
}

static void print_dashboard(void){
    printf("\f\n Analog readings from mcp3008  \n\n");
    for (int i = 0; sensors[i].label; i++) {
        int reading = mcp3008_read(sensors[i].channel);
        if (i >= 2) printf("\n\n"); // two joystick readings on same line
        print_analog_scale(reading, sensors[i].label, sensors[i].color, sensors[i].min);
    }
}

void main(void)  {
    timer_init();
    uart_init();
    mcp3008_init();

    while (1) {
        print_dashboard();
        timer_delay_ms(100);
    }
}
