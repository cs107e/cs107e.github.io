#include "assert.h"
#include "gpio.h"
#include "uart.h"
#include "printf.h"
#include "strings.h"

// pinout for rotary encoder
//    Switch NC Switch
//           o
//    Clock Gnd Data
//
// Clock sometimes referred to as pin "A" and Data pin "B"

static const gpio_id_t RE_CLOCK = GPIO_PB4;
static const gpio_id_t RE_DATA = GPIO_PB3;

#define MAX_POS 32
#define CLOCKWISE 1
#define COUNTER_CLOCKWISE -1

static void print_rotary(int cur) {
    char buf[MAX_POS+1];
    assert(cur <= MAX_POS);
    memset(buf, ' ', sizeof(buf));
    memset(buf, '#', cur);
    buf[sizeof(buf)-1] = '\0';
    printf("[%s] = %d\n", buf, cur);
}

void main(void) {
    uart_init();
    printf("Starting program %s\n\n", __FILE__);

    gpio_set_input(RE_CLOCK);
    gpio_set_input(RE_DATA);

    int pos = MAX_POS/2;
    print_rotary(pos);
    while (pos != 0 && pos != MAX_POS) { // exit when pos reach either extreme
        int a = gpio_read(RE_CLOCK);
        if (a == 0) { // when clock pin goes low
            int bBefore = gpio_read(RE_DATA);   // observe change in data pin
            while (gpio_read(RE_CLOCK) == 0);   // as clock pin rises
            int bAfter = gpio_read(RE_DATA);
            if (bBefore != bAfter) { // data is 90 deg out of phase with clock
                pos += (bAfter > bBefore ? CLOCKWISE : COUNTER_CLOCKWISE);
                print_rotary(pos);
            }
        }
    }
    printf("\nExiting program %s\n", __FILE__);
}