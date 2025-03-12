#include "gpio.h"
#include "timer.h"
#include "midi.h"
// #include "pi.h"
// #include "twinkle-guitar.h"
#include "nggyu.h"
// #include "let-it-go.h"
// #include "touch-the-sky.h"


#define CHANNEL_KEY    0
#define CHANNEL_BEATS  9

void main() {
    gpio_init();
    timer_init();
    midi_init();

    while (1) {
        // play_midi(midi_data, sizeof(midi_data) / sizeof(midi_data[0]), tempo);
        play_midi(midi_data, 500000);
        timer_delay_ms(4000);
    }
}
