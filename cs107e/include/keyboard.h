#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "gpio.h"
#include "ps2.h"

/*
 * Module to read keys typed on a PS/2 keyboard.
 *
 * You implement this module in assignments 5 and 7.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 *
 * Last updated:   February 2019
 */

typedef struct {
    enum { KEY_PRESS, KEY_RELEASE } what;
    unsigned char keycode;
} key_action_t;

/*
 * These bit flags are used for the state of the various modifier
 * keys on the keyboard.
 */
typedef enum {
    KEYBOARD_MOD_SHIFT = 1 << 0,
    KEYBOARD_MOD_ALT = 1 << 1,
    KEYBOARD_MOD_CTRL = 1 << 2,
    KEYBOARD_MOD_CAPS_LOCK = 1 << 3,
    KEYBOARD_MOD_SCROLL_LOCK = 1 << 4,
    KEYBOARD_MOD_NUM_LOCK = 1 << 5,
} keyboard_modifiers_t;

typedef struct {
    key_action_t action;                // see struct declared above
    ps2_key_t key;                      // entry taken from ps2_keys table (see ps2.h)
    keyboard_modifiers_t modifiers;     // modifiers in effect, composed of above bit flags
} key_event_t;


#define KEYBOARD_CLOCK GPIO_PIN3
#define KEYBOARD_DATA GPIO_PIN4

/*
 * `keyboard_init`: Required initialization for keyboard
 *
 * The keyboard must first be initialized before any key events can be read.
 * The first and second arguments identify which GPIO pins to use for the
 * PS/2 clock and data lines, respectively.
 *
 * Although `keyboard_init` configures the requested clock and data GPIOs
 * to use the internal pull-up resistors, it is recommended to choose GPIO
 * pins whose default state is already pull-up, such as KEYBOARD_CLOCK and
 * KEYBOARD_DATA defined above. This avoid timing issues due to the keyboard
 * attempting to handshake with the Pi before `keyboard_init` has executed.
 */
void keyboard_init(unsigned int clock_gpio, unsigned int data_gpio);


/*
 * `keyboard_read_next`: Top level keyboard interface.
 *
 * This function reads (blocking) the next key typed on the keyboard.
 * The character returned reflects the current keyboard modifier settings.
 *
 * Return values in the range 0 - 0x7f indicate the typed key is an ordinary
 * Ascii character. For a typed key not associated with an Ascii character,
 * such an arrow or function key, the function returns a value >= 0x90. The
 * value assigned to each non-Ascii key is given in the list of `ps2_codes`
 * in the `ps2.h` header file.
 *
 * This function calls `keyboard_read_event` to receive a key press event.
 */
unsigned char keyboard_read_next(void);


/*
 * `keyboard_read_event`: Mid level keyboard interface.
 *
 * The function reads (blocking) the next key event.
 * Returns a `key_event_t` struct that represents the key event.
 * A key event is a press or release of a single key. The returned
 * struct includes the key that was pressed or released and the state
 * of the modifier flags.
 * 
 * Returned key events do not include modifier keys. Pressing shift,
 * for example, will not result in a key event being returned, but if a key
 * that does produce a key event (e.g., 'a') is pressed before the shift
 * is released, `keyboard_read_event` will return an event for this
 * key with the shift modifier set.
 *
 * This function calls `keyboard_read_sequence` to read a sequence.
 */
key_event_t keyboard_read_event(void);


/*
 * `keyboard_read_sequence`: Low level keyboard interface.
 *
 * Reads a sequence of scancode bytes corresponding to the press or
 * release of a single key.  Returns a `key_action_t` struct that
 * represents the key action for the sequence read. Reads 1, 2, or 3
 * scancodes:
 *    1 byte:  ordinary key press
 *    2 bytes: ordinary key release or extended key press
 *    3 bytes: extended key release
 *
 * The `keycode` field of the returned key_action_t stores the last byte
 * of the sequence. This keycode identifies the PS/2 key that was acted upon.
 *
 * This function calls `keyboard_read_scancode` to read each scancode.
 */
key_action_t keyboard_read_sequence(void);


/*
 * `keyboard_read_scancode`: Bottom level keyboard interface.
 *
 * Read (blocking) a single scancode from the PS/2 keyboard.
 * Bits are read on the falling edge of the clock.
 *
 * Reads 11 bits: 1 start bit, 8 data bits, 1 parity bit, and 1 stop bit
 *
 * Discards and restarts the scancode if:
 *   (lab5) The start bit is incorrect
 *   (assign5) or if parity or stop bit is incorrect
 *
 * Returns the 8 data bits of a well-formed PS/2 scancode.
 * Will not return until it reads a complete and valid scancode.
 */
unsigned char keyboard_read_scancode(void);


/*
 * `keyboard_use_interrupts`
 *
 * Change keyboard from default polling behavior to instead configure interrupts
 * for gpio events. After setting keyboard to use interrupts, client must
 * also globally enable interrupts at system level.
 */
void keyboard_use_interrupts(void);


#endif
