/*
    Adafruit LSM6DS3TR + LIS3MDL - Precision 9 DoF IMU
    Accel + Gyro + Magnetometer
    https://www.adafruit.com/product/5543
    Datasheet https://cdn-learn.adafruit.com/downloads/pdf/adafruit-lsm6ds3tr-c-lis3mdl-precision-9-dof-imu.pdf

    Sample code to read the accelerometer in LSD6DS33.c
 */

#include "LSM6DS33.h"
#include "assert.h"
#include "i2c.h"

enum reg_address {
    WHO_AM_I  = 0x0F,
    CTRL1_XL  = 0x10,
    CTRL8_XL  = 0x17,
    CTRL9_XL  = 0x18,
    OUTX_L_XL = 0x28,
    OUTX_H_XL = 0x29,
    OUTY_L_XL = 0x2A,
    OUTY_H_XL = 0x2B,
    OUTZ_L_XL = 0x2C,
    OUTZ_H_XL = 0x2D,
};

static const unsigned MY_I2C_ADDR = 0x6A; // confirm device id, components can differ!

static void write_reg(unsigned char reg, unsigned char val) {
	unsigned char data[2] = {reg, val};
	i2c_write(MY_I2C_ADDR, data, 2);
}

static unsigned read_reg(unsigned char reg) {
	i2c_write(MY_I2C_ADDR, &reg, 1);
	unsigned char val = 0;
	i2c_read(MY_I2C_ADDR, &val, 1);
	return val;
}

void lsm6ds33_init(void) {
    unsigned id = read_reg(WHO_AM_I);  // confirm id, expect 0x69
    assert(id == 0x69);
	write_reg(CTRL1_XL, 0x80);  // 1600Hz (high perf mode)
    // accelerator _XL registers
    write_reg(CTRL9_XL, 0x38);  // ACCEL: x,y,z enabled (bits 4-6)
}

void lsm6ds33_read_accelerometer(short *x, short *y, short *z) {
    *x =  read_reg(OUTX_L_XL);
    *x |= read_reg(OUTX_H_XL) << 8;
    *y =  read_reg(OUTY_L_XL);
    *y |= read_reg(OUTY_H_XL) << 8;
    *z =  read_reg(OUTZ_L_XL);
    *z |= read_reg(OUTZ_H_XL) << 8;
}
