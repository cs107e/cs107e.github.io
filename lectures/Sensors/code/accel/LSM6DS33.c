/*
  MinIMU-9 v5 
    https://www.pololu.com/product/2738

  Raspberry Pi I2C
	SDA = gpio pin2 
	SCL = gpio pin3

    void i2c_init(void);
    void i2c_read(unsigned slave_address, char *data, int data_length);
    void i2c_write(unsigned slave_address, char *data, int data_length);

    i2c slave address;
  	  LSM6DS33 (gyro and accelerometer)	1101011b
	  LIS3MDL (magnetometer)    			0011110b

 */
#include "timer.h"
#include "printf.h"
#include "i2c.h"
#include "LSM6DS33.h"

const unsigned lsm6ds33_address = 0b1101011; // this is for the gyro/accel;

void lsm6ds33_write_reg(unsigned char reg, unsigned char v) {
	char data[2] = {reg, v};
	i2c_write(lsm6ds33_address, data, 2);
}

unsigned lsm6ds33_read_reg(unsigned char reg) {
	i2c_write(lsm6ds33_address, (void*) &reg, 1);
	unsigned char uc = 0;
	i2c_read(lsm6ds33_address, (void*) &uc, 1);
	return uc;
}

void lsm6ds33_init() {
	lsm6ds33_write_reg(CTRL2_G, 0x80);   // 1600Hz (high perf mode)
	lsm6ds33_write_reg(CTRL1_XL, 0x80);  // 1600Hz (high perf mode)
}

unsigned lsm6ds33_get_whoami() {
	// should return 0x69 ...
    return lsm6ds33_read_reg(WHO_AM_I);
}

// default is enabled
void lsm6ds33_enable_gyroscope() {
	// gyroscope _G registers
	lsm6ds33_write_reg(CTRL10_C, 0x38);  // GYRO: x,y,z enabled (bits 4-6)
}

// default is enabled
void lsm6ds33_enable_accelerometer() {
    // accelerator _XL registers 
	lsm6ds33_write_reg(CTRL9_XL, 0x38);  // ACCEL: x,y,z enabled (bits 4-6)
}

void lsm6ds33_read_gyroscope(short *x, short *y, short *z) {
    *x =  lsm6ds33_read_reg(OUTX_L_G);
    *x |= lsm6ds33_read_reg(OUTX_H_G) << 8;

    *y =  lsm6ds33_read_reg(OUTY_L_G);
    *y |= lsm6ds33_read_reg(OUTY_H_G) << 8;

    *z =  lsm6ds33_read_reg(OUTZ_L_G);
    *z |= lsm6ds33_read_reg(OUTZ_H_G) << 8;
}

void lsm6ds33_read_accelerometer(short *x, short *y, short *z) {
    *x =  lsm6ds33_read_reg(OUTX_L_XL);
    *x |= lsm6ds33_read_reg(OUTX_H_XL) << 8;
	
    *y =  lsm6ds33_read_reg(OUTY_L_XL);
    *y |= lsm6ds33_read_reg(OUTY_H_XL) << 8;

    *z =  lsm6ds33_read_reg(OUTZ_L_XL);
    *z |= lsm6ds33_read_reg(OUTZ_H_XL) << 8;
}

