/*
 * Julie Zelenski, Feb 2024
 * Sample program to drive strip of Neopixels (WS2811/2812 LEDs)
 */
#include "gl.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

#define DATA_PIN GPIO_PB4
#define PB_DATA (unsigned int *)0x02000040
#define BIT_FOR_PB4 (1 << 4)

// this function written in assembly for precise timing, see neo_timing.s
void send_bit(unsigned int *data, unsigned int data_bit_to_flip, unsigned int zero_or_one);

static void send_byte(uint8_t byte) {
    for (uint8_t mask = 1 << 7; mask != 0 ; mask >>= 1 ) {
        send_bit(PB_DATA, BIT_FOR_PB4, (byte & mask) != 0);
    }
    // inter-bit gap not v sensitive, but must not exceed latch delay (see below)    
}

static void send_pixel_color(color_t c)   {
    // neopixel bytes sent in order G-R-B
    send_byte(c >> 8);  // G 
    send_byte(c >> 16); // R
    send_byte(c);       // B
    // inter-byte gap not v sensitive, but must not exceed latch delay (see below)    
}

void main(void)  {
    gpio_init();
    timer_init();
    uart_init();
    gpio_set_output(DATA_PIN);

    int parity = 0, nleds = 8;
    while (1) {
        for (int i = 0; i < nleds; i++) {
            color_t c = (i % 2 == parity? GL_RED : GL_WHITE);
            send_pixel_color(c);
        }
        // after send sequence, hold data low for pixels to latch and display
        // delay >= 55us (shorter might not work, longer totally fine)
        timer_delay_ms(200); // delay for latch (edit for faster/slower frame rate)
        parity ^= 1;
    }
}
