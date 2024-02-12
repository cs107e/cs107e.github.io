#ifndef KEYBOARD_H
#define KEYBOARD_H

/*
 * Module to read keys typed on a PS/2 keyboard.
 *
 * Students implement this module in assignment 5.
 *
 * Author: Philip Levis <pal@cs.stanford.edu>
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "gpio.h"
#include "ps2_keys.h"

/*
 * Type: `key_action_t`
 *
 * This struct represents a single action on a PS2 key.
 * The action is either a press or release of the specified key
 * (identified by its keycode).
 */
typedef struct {
    enum { KEY_PRESS, KEY_RELEASE } what;
    unsigned char keycode;
} key_action_t;

/*
 * Type: `keyboard_modifiers_t`
 *
 * This enum type represents the state of the modifier keys.
 * A single bit flag is used for each modifier. The bits
 * taken together as a bitset indicate the state of all modifier keys.
 * A bit can be set/clear/read within the bitset by using the individual
 * bit flag as a mask.
 */
typedef enum {
    KEYBOARD_MOD_SHIFT = 1 << 0,
    KEYBOARD_MOD_ALT = 1 << 1,
    KEYBOARD_MOD_CTRL = 1 << 2,
    KEYBOARD_MOD_CAPS_LOCK = 1 << 3,
} keyboard_modifiers_t;

/*
 * Type: `key_event_t`
 *
 * This struct represents a single key event, which packages
 * the raw key action with additional data about keyboard state.
 */
typedef struct {
    key_action_t action;                // see struct declared above
    ps2_key_t key;                      // entry taken from ps2_keys table (see ps2_keys.h)
    keyboard_modifiers_t modifiers;     // modifiers in effect, composed of above bit flags
} key_event_t;

#define KEYBOARD_CLOCK GPIO_PG12
#define KEYBOARD_DATA GPIO_PB7

/*
 * `keyboard_init`: Required initialization for keyboard.
 *
 * The keyboard must first be initialized before any key events can be read.
 * The two arguments are the ids of the GPIOs connected to the PS2 clock and data lines.
 *
 * @param clock    the gpio connected to the clock line of keyboard
 * @param data     the gpio connected to the data line of keyboard
 */
void keyboard_init(gpio_id_t clock, gpio_id_t data);

/*
 * `keyboard_read_next`: Top level keyboard interface.
 *
 * This function reads (blocking) the next key typed on the keyboard.
 * The character returned reflects the current keyboard modifiers in effect.
 *
 * Return values in the range 0 - 0x7f indicate the typed key is an ordinary
 * Ascii character. For a typed key not associated with an Ascii character,
 * such an arrow or function key, the function returns a value >= 0x90. The
 * value assigned to each non-Ascii key is given in the list of `ps2_special_chars`
 * in the `ps2_keys.h` header file.
 *
 * This function calls `keyboard_read_event` to receive a key press event.
 *
 * @return      Ascii value of typed char or function code for non-ascii key
 */
unsigned char keyboard_read_next(void);

/*
 * `keyboard_read_event`: Mid level keyboard interface.
 *
 * The function reads (blocking) the next key event.
 * Returns a `key_event_t` struct that represents the key event.
 * A key event is a press or release of a single non-modifier key.
 * The returned struct includes the ps2 key that was pressed or
 * released and the keyboard modifiers in effect.
 *
 * `keyboard_read_event` does not return until it observes a key
 * event for a non-modifier key. Pressing or releasing a modifier
 * key changes the state of the modifiers for other key events
 * but does not produce its own key event. For example, if the user
 * presses Shift, press 'a', release Shift, and release 'a', this
 * comprises four key actions (four calls to `keyboard_read_sequence`)
 * but only two key events (press 'a' and release `a`). The key
 * event returned for press 'a' has the shift modifier on, the key event
 * returned for release 'a' has shift modifier off.
 *
 * This function calls `keyboard_read_sequence` to read a sequence.
 *
 * @return      key_event_t struct containing key event information
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
 * of the sequence. This keycode identifies the PS2 key that was acted upon.
 *
 * This function calls `keyboard_read_scancode` to read each scancode.
 *
 * @return      key_action_t struct containing key action for sequence
 */
key_action_t keyboard_read_sequence(void);

/*
 * `keyboard_read_scancode`: Bottom level keyboard interface.
 *
 * Calls into ps2 module to read (blocking) a single scancode from
 * the PS2 keyboard device.
 *
 * @return      scancode read from keyboard
 */
unsigned char keyboard_read_scancode(void);

/*
 * `keyboard_use_interrupts` : reference only
 *
 * Change keyboard from default polling behavior to instead configure interrupts
 * for gpio events. After setting keyboard to use interrupts, client must
 * also globally enable interrupts at system level. This switchable feature is
 * specific to reference module. The student's keyboard module is initially
 * polling-only (assign5) and later changed to interrupt-only (assign7).
 */
void keyboard_use_interrupts(void);

#endif
