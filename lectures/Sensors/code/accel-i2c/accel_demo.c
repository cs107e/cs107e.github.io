#include "assert.h"
#include "i2c.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"

/*
    Adafruit LSM6DS3TR + LIS3MDL - Precision 9 DoF IMU
    Accel + Gyro + Magnetometer
    https://www.adafruit.com/product/5543
    Datasheet https://cdn-learn.adafruit.com/downloads/pdf/adafruit-lsm6ds3tr-c-lis3mdl-precision-9-dof-imu.pdf

    Sample code to read the accelerometer via i2c
 */

struct axes {
    short x, y, z; // expressed in mg
};


void accel_config(i2c_device_t *dev) {
    const unsigned char CHIP_ID = 0x0F;
    const unsigned char EXPECTED = 0x69;
    assert(i2c_read_reg(dev, CHIP_ID) == EXPECTED);
    const unsigned char CTRL1_XL = 0x10;
    i2c_write_reg(dev, CTRL1_XL, 0x80);   // 1600Hz (high perf mode)
    const unsigned char CTRL9_XL = 0x18;
    i2c_write_reg(dev, CTRL9_XL, 0b111000);  // Enable x,y,z accel (bits 4-6)
}

void accel_read(i2c_device_t *dev, struct axes *p) {
    const unsigned char OUT_ACCEL = 0x28;
    unsigned char bytes[6];
    i2c_read_reg_n(dev, OUT_ACCEL, bytes, 6);
    // data is 6 bytes (2 byte x/y/z little endian)
    short *s = (short *)bytes; // no swizzle needed, access as signed short
    short divisor = 16;  // 16384 is 1g (1g == 1000mg)
    p->x = s[0]/divisor;
    p->y = s[1]/divisor;
    p->z = s[2]/divisor;
}

void main(void) {
    timer_init();
	uart_init();

    i2c_init();
    i2c_device_t *dev = i2c_new(0x6a); // address for Adafruit version
    accel_config(dev);

	while (1) {
        struct axes a;
        accel_read(dev, &a);
        printf("accel x,y,z = (%4dmg,%4dmg,%4dmg)\n", a.x, a.y, a.z);
        timer_delay_ms(300);
	}
}

