#pragma once
#include "mouse.h"
void ref_mouse_init(unsigned int clock_gpio, unsigned int data_gpio);
mouse_event_t ref_mouse_read_event(void);
