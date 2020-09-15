#ifndef KEYBOARD_REF
#define KEYBOARD_REF
#include "keyboard.h"
void ref_keyboard_init(unsigned int clock_gpio, unsigned int data_gpio);
unsigned char ref_keyboard_read_next(void);
key_event_t ref_keyboard_read_event(void);
key_action_t ref_keyboard_read_sequence(void);
unsigned char ref_keyboard_read_scancode(void);
void ref_keyboard_use_interrupts(void);
#endif
