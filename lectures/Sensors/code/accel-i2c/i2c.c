/*
     simple implementation of i2c using bit bang
     Not sure if properly conforms to timing specs
     Missing:  bus arbitration (assumes can master), clock stretch, error checking, reset

     Author: Julie Zelenski
     Tue Feb 13 17:44:05 PST 2024
     Minor updates May 14 2024
 */
#include "i2c.h"
#include "assert.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "malloc.h"
#include <stddef.h>
#include "strings.h"
#include "timer.h"

static struct {
    gpio_id_t sda;
    gpio_id_t scl;
} const module = { .sda = GPIO_PG13, .scl = GPIO_PG12 };

enum { WRITE_BIT = 0, READ_BIT = 1};

enum { ACK = 0, NAK = 1 };

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

// returns false if error
static bool write_byte(unsigned char byte) {
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
    int response = gpio_read(module.sda); // ack from slave
    bool success = (response == ACK);
    gpio_write(module.scl, 0); // clock low
    timer_delay_us(1);
    gpio_set_output(module.sda);
    gpio_write(module.sda, 0); // data low
    timer_delay_us(20); // pause for debugging
    return success;
}

static int read_byte(bool last) { // if last, respond with NAK in place of ACK
    int val = 0;
    gpio_set_input(module.sda);
    for (int j = 0; j < 8; j++) {
        gpio_write(module.scl, 1); // clock hi
        timer_delay_us(1);
        int bit = gpio_read(module.sda); // read SDA while clock high
        val |= (bit << (8-j-1));
        gpio_write(module.scl, 0); // clock lo
        timer_delay_us(1);
    }
    gpio_set_output(module.sda);
    gpio_write(module.sda, last? NAK : ACK); // NAK or ACK
    gpio_write(module.scl, 1); // clock hi
    timer_delay_us(1);
    gpio_write(module.scl, 0); // clock lo
    timer_delay_us(1);
    gpio_write(module.sda, 0); // data low
    timer_delay_us(20); // pause for debugging
    return val;
}

static bool block_write(unsigned char device_id, unsigned char *data, int data_length) {
    bool success = false;
    start();
    success = write_byte((device_id << 1) | WRITE_BIT);
    for (int i = 0; i < data_length && success; i++) {
        success = write_byte(data[i]);
    }
    stop();
    return success;
}

static bool block_read(unsigned char device_id, unsigned char *data, int data_length) {
    bool success = false;
    start();
    success = write_byte((device_id << 1) | READ_BIT);
    for (int i = 0; i < data_length && success; i++) {
        data[i] = read_byte(i == data_length - 1);
    }
    stop();
    return success;
}

struct i2c_device {
    unsigned char addr;
};

i2c_device_t * i2c_new(unsigned char addr) {
    if (!block_write(addr, 0, 0)) {
        error("Unable to communicate with i2c device at address");
        return NULL;
    }
    i2c_device_t *dev = malloc(sizeof(*dev));
    dev->addr = addr;
    return dev;
}

bool i2c_write_reg(i2c_device_t *dev, unsigned char reg, unsigned char val) {
    unsigned char buf[2] = {reg , val};
    return block_write(dev->addr, buf, sizeof(buf));
}

bool i2c_write_reg_n(i2c_device_t *dev, unsigned char reg, unsigned char *bytes, int n) {
    unsigned char buf[n+1];
    buf[0] = reg;
    memcpy(buf+1, bytes, n);
    return block_write(dev->addr, buf, sizeof(buf));
}

unsigned char i2c_read_reg(i2c_device_t *dev, unsigned char reg) {
    unsigned char buf[1];
    if (!block_write(dev->addr, &reg, 1)) return 0xff;
    if (!block_read(dev->addr, buf, 1)) return 0xff;
    return buf[0];
}

bool i2c_read_reg_n(i2c_device_t *dev, unsigned char reg, unsigned char *bytes, int n) {
    if (!block_write(dev->addr, &reg, 1)) return false;
    return block_read(dev->addr, bytes, n);
}
