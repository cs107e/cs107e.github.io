/*
     simple implementation of i2c using bit bang
     Not sure if properly conforms to timing specs
     Missing:  bus arbitration (assumes can master), clock stretch, error checking, reset

     Author: Julie Zelenski
     Tue Feb 13 17:44:05 PST 2024
 */
#include "i2c.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "timer.h"

static struct {
    gpio_id_t sda;
    gpio_id_t scl;
} const module = { .sda = GPIO_PG13, .scl = GPIO_PG12 };

enum { WRITE_BIT = 0, READ_BIT = 1};

void i2c_init(void) {
    gpio_set_input(module.scl);
    gpio_set_input(module.sda);
    gpio_set_pullup(module.scl);
    gpio_set_pullup(module.sda);
}

static void start(void) {
    gpio_set_output(module.scl);
    gpio_set_output(module.sda);
    gpio_write(module.sda, 1);
    gpio_write(module.scl, 1);
    gpio_write(module.sda, 0); // start: SDA goes low while SCL is high
    gpio_write(module.scl, 0);
}

static void stop(void) {
    gpio_write(module.sda, 0);
    gpio_write(module.scl, 1); // stop: SDA goes high afer SCL does
    gpio_write(module.sda, 1);
    gpio_set_input(module.scl);
    gpio_set_input(module.sda);
}

static void write_byte(unsigned char byte) {
    for (int j = 0; j < 8; j++) {
        int bit = (byte & (1 << (8-j-1)))  >> (8-j-1);
        gpio_write(module.sda, bit); // change SDA while clock low
        gpio_write(module.scl, 1); // clock hi
        timer_delay_us(1);
        gpio_write(module.scl, 0); // clock lo
        timer_delay_us(1);
    }
    gpio_set_input(module.sda);
    gpio_write(module.scl, 1); // clock hi
    timer_delay_us(5);
    gpio_read(module.sda); // ack from slave
    gpio_write(module.scl, 0); // clock low
    timer_delay_us(1);
    gpio_set_output(module.sda);
    gpio_write(module.sda, 0); // data low
    timer_delay_ms(1); // pause for debugging
}

static int read_byte(bool last) { // if last, respond with NAK in place of ACK
    unsigned char byte = 0;
    gpio_set_input(module.sda);
    for (int j = 0; j < 8; j++) {
        gpio_write(module.scl, 1); // clock hi
        timer_delay_us(1);
        int bit = gpio_read(module.sda); // read SDA while clock high
        byte |= (bit << (8-j-1));
        gpio_write(module.scl, 0); // clock lo
        timer_delay_us(1);
    }
    gpio_set_output(module.sda);
    gpio_write(module.sda, last? 1 : 0); // NAK or ACK
    gpio_write(module.scl, 1); // clock hi
    timer_delay_us(1);
    gpio_write(module.scl, 0); // clock lo
    timer_delay_us(1);
    gpio_write(module.sda, 0); // data low
    timer_delay_ms(1); // pause for debugging
    return byte;
}

void i2c_write(unsigned char device_id, unsigned char *data, int data_length) {
   start();
   write_byte((device_id << 1) | WRITE_BIT);
    for (int i = 0; i < data_length; i++) {
       write_byte(data[i]);
    }
   stop();
}

void i2c_read(unsigned char device_id, unsigned char *data, int data_length) {
    start();
    write_byte((device_id << 1) | READ_BIT);
    for (int i = 0; i < data_length; i++) {
        data[i] = read_byte(i == data_length - 1);
    }
    stop();
}
