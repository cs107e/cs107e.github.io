#pragma once
#include "ps2.h"
ps2_device_t *ps2ref__new(unsigned int clock_gpio, unsigned int data_gpio);
unsigned char ps2ref__read(ps2_device_t *dev);
bool ps2ref__write(ps2_device_t *dev, unsigned char command);
void ps2ref__use_interrupts(ps2_device_t *dev);
