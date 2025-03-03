/*
 * Gy-521 3-axis accelerometer+gyroscope i2c sensor
 * Sample code below shows raw accel readings, no calibration/correction
 *
 * https://componentslibrary.com/gy-521-datasheet.html
 * https://protosupplies.com/product/mpu-6050-gy-521-3-axis-accel-gryo-sensor-module/
 */
#include "assert.h"
#include "i2c.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

typedef struct {
    short x, y, z; // expressed in mg
} axes_t;

enum gy521_register {
    GY_WHO_AM_I     = 0x75,
    CONFIG_ACCEL    = 0x1C,
    READINGS_ACCEL  = 0x3B,
    PWR_MGMT_1      = 0x6B,
};

static i2c_device_t *gy521_init(void) {
    uint8_t addr = 0x68; // confirm device addr with datasheet, components can differ!
    i2c_device_t *dev = i2c_new(addr);
    const uint8_t EXPECTED = addr; // expect device addr
    assert(i2c_read_reg(dev, GY_WHO_AM_I) == EXPECTED);
    i2c_write_reg(dev, PWR_MGMT_1, 0); // wake from sleep
    i2c_write_reg(dev, CONFIG_ACCEL, 0); // config accel for range -+2g, scale factor 16384
    return dev;
}

short extract_reading(uint8_t *bytes) {
    short scale_factor = 16384;  // 16384 is 1g (1g == 1000mg)
    short reading = ((int8_t)bytes[0] << 8) + bytes[1];
    return (1000 * reading)/scale_factor;
}

void gy521_read(i2c_device_t *dev, axes_t *p) {
    uint8_t bytes[6]; // accel readings: x/y/z 2-byte big-endian
    i2c_read_reg_n(dev, READINGS_ACCEL, bytes, sizeof(bytes));
    p->x = extract_reading(&bytes[0]);
    p->y = extract_reading(&bytes[2]);
    p->z = extract_reading(&bytes[4]);
}

void main(void) {
    uart_init();
    i2c_init();
    i2c_device_t *gy = gy521_init();
    while (1) {
        axes_t a;
        gy521_read(gy, &a);
        printf("Reading  x= %5dmg, y= %5dmg, z= %5dmg)\n", a.x, a.y, a.z);
        timer_delay(1);
    }
}
