#pragma once
#include <stdbool.h>

typedef struct i2c_device i2c_device_t;

void i2c_init(void); // once to init entire i2c module
i2c_device_t * i2c_new(unsigned char addr); // per each i2c device

bool i2c_write_reg(i2c_device_t *dev, unsigned char reg, unsigned char val);
bool i2c_write_reg_n(i2c_device_t *dev, unsigned char reg, unsigned char *bytes, int n);

unsigned char i2c_read_reg(i2c_device_t *dev, unsigned char reg);
bool i2c_read_reg_n(i2c_device_t *dev, unsigned char reg, unsigned char *bytes, int n);
