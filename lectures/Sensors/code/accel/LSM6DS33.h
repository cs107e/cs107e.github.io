#pragma once
#include <stdbool.h>

void lsm6ds33_init(void);
void lsm6ds33_read_accelerometer(short *x, short *y, short *z);
