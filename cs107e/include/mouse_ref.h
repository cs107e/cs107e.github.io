#ifndef MOUSE_REF
#define MOUSE_REF
#include "mouse.h"
bool ref_mouse_init(unsigned int clock_gpio, unsigned int data_gpio);
mouse_event_t ref_mouse_read_event(void);
#endif
