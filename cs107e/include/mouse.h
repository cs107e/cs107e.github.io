#ifndef MOUSE_H
#define MOUSE_H

/*
 * Module to read mouse events from a PS/2 mouse.
 *
 * Students implement this module as an extension to assignment 7.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "gpio.h"
#include <stdbool.h>

/*
 * Each event corresponds to a particular user action with
 * mouse (press/release button, move/drag). This enum
 * identifes the four types of mouse actions.
 */
typedef enum {
    MOUSE_BUTTON_PRESS = 0,
    MOUSE_BUTTON_RELEASE = 1,
    MOUSE_MOVED = 2,
    MOUSE_DRAGGED = 3,
} mouse_action_t;

/*
 * This struct captures the information associated with a mouse event.
 * For information on the value and purpose of the fields, refer to
 * the documentation for function `mouse_read_event` below.
 */
typedef struct {
    mouse_action_t action;
    int dx, dy;
    bool x_overflow, y_overflow;
    bool left, middle, right;
} mouse_event_t;

#define MOUSE_CLOCK GPIO_PD21
#define MOUSE_DATA GPIO_PD22

/*
 * `mouse_init`: Required initialization for mouse.
 *
 * The mouse must first be initialized before any mouse events can be read.
 * The two arguments are the ids of the GPIOs connected to the PS2 clock and data lines.
 * `mouse_init` resets the mouse and enables data reporting mode.
 *
 * @param clock    the gpio connected to the clock line of mouse
 * @param data     the gpio connected to the data line of mouse
 */
void mouse_init(gpio_id_t clock, gpio_id_t data);

/*
 * `mouse_read_event`
 *
 * The function reads (blocking) the next event from the mouse.
 * Returns a `mouse_event_t` struct that represents the mouse state/action.
 * The dx, dy fields are delta values, i.e. relative change in x and y
 * when mouse is moved/dragged. A delta value is within the range -255 to +255.
 * The x_overflow/y_overflow fields indicate an overflow occurred in
 * computing the delta value. If actual change dx or dy exceeds
 * representable range (> 255), the overflow field is true, false otherwise.
 * The left/middle/right fields give the state of the mouse buttons.
 * The field is true if the corresponding mouse button is down, false otherwise.
 *
 * @return         mouse_event_t struct containing event data
 */
mouse_event_t mouse_read_event(void);

#endif
