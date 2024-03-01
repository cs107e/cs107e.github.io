/*
    quick and dirty implementation of spi using bit bang
    Author: Julie Zelenski
 */
#include "spi.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "timer.h"

static struct {
    gpio_id_t sck;
    gpio_id_t cs;
    gpio_id_t mosi;
    gpio_id_t miso;
} const module = { .cs = GPIO_PD10, .sck = GPIO_PD11, .mosi = GPIO_PD12, .miso = GPIO_PD13 };

void spi_init(void) {
    gpio_set_input(module.miso);
    gpio_set_output(module.mosi);
    gpio_set_output(module.sck);
    gpio_set_output(module.cs);
    gpio_write(module.sck, 0); // CLK idle low
    gpio_write(module.cs, 1); // CS idle high
    timer_delay(1);
}

static unsigned char transfer_one_byte(unsigned char byte) {
    unsigned char read = 0;
    for (int j = 7; j >= 0; j--) {
        int bit = (byte & (1 << j));
        gpio_write(module.mosi, bit);
        gpio_write(module.sck, 1); // pulse clock
        bit = gpio_read(module.miso);
        read = (read << 1) | bit;
        gpio_write(module.sck, 0);
    }
    return read;
}

void spi_transfer(unsigned char *in, unsigned char *out, int len) {
    gpio_write(module.cs, 0); // start: bring CS low
    timer_delay_us(1);
    for (int i = 0; i < len; i++) {
        out[i] = transfer_one_byte(in[i]);
        timer_delay_us(5);
    }
    gpio_write(module.cs, 1); // stop: restore CS high
    timer_delay_us(100);
}
