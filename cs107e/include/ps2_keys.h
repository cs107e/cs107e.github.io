#ifndef PS2_KEYS_H
#define PS2_KEYS_H

/*
 * This module declares constants for interacting with a PS/2
 * keyboard, including an array that serves as a lookup table
 * to access information about the keys and the characters they produce.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Author: Philip Levis <pal@cs.stanford.edu>
 */

#define PS2_CODE_RELEASE 0xF0   // Scancodes used in the PS/2 protocol
#define PS2_CODE_EXTENDED 0xE0

/*
 * Type: `ps2_key_t`
 *
 * A ps2_key_t struct has two fields, `ch` and `other_ch`.
 * These fields correspond to the unmodified and modified character produced by
 * a given PS2 key. For example, the A key produces `{ 'a', 'A' }` and the
 * Four key produces `{ '4', '$' }`. Keys such as Tab that are unchanged
 * by the modifier have same `ch` and `other_ch`, e.g. `{'\t', '\t'}`.
 * A non-ASCII key produces a char with special value >= 0x90 as
 * assigned in the enumeration of special characters below.
 */
typedef struct {
    unsigned char ch;
    unsigned char other_ch;
} ps2_key_t;

/*
 * Global constant array: `ps2_keys`
 *
 * Each entry in the array corresponds to one physical key on the keyboard.
 * Each key has an assigned PS/2 scancode. The array is organized
 * in order of scancode. The scancode is used as an index to access
 * the characters produced by that key.
 * As an example, pressing the A key on a PS2 keyboard generates scancode `0x1c`.
 * Accessing the array element at that index `ps2_keys[0x1c]` retrieves the ps2_key_t
 * `{'a', 'A'}`. These are the unmodified and modified characters produced by the A key.
 */
extern ps2_key_t const ps2_keys[];

/*
 * Enumeration of special chars
 *
 * Regular chars have ASCII characters values that are all <= 0x7F,
 * we assign special character codes >= 0x90 to be produced by
 * the non-ASCII keys.
 */
enum {
    PS2_KEY_NONE = 0,
    PS2_KEY_SHIFT = 0x90,
    PS2_KEY_ALT,    // enum values assigned in increasing sequence from here
    PS2_KEY_CTRL,
    PS2_KEY_CAPS_LOCK,
    PS2_KEY_ENTER,
    PS2_KEY_ESC,
    PS2_KEY_F1,
    PS2_KEY_F2,
    PS2_KEY_F3,
    PS2_KEY_F4,
    PS2_KEY_F5,
    PS2_KEY_F6,
    PS2_KEY_F7,
    PS2_KEY_F8,
    PS2_KEY_F9,
    PS2_KEY_F10,
    PS2_KEY_F11,
    PS2_KEY_F12,
    PS2_KEY_NUM_LOCK,
    PS2_KEY_HOME,
    PS2_KEY_PAGE_UP,
    PS2_KEY_PAGE_DOWN,
    PS2_KEY_INSERT,
    PS2_KEY_DELETE,
    PS2_KEY_END,
    PS2_KEY_SCROLL_LOCK,
    PS2_KEY_ARROW_UP,
    PS2_KEY_ARROW_DOWN,
    PS2_KEY_ARROW_LEFT,
    PS2_KEY_ARROW_RIGHT
};

#endif
