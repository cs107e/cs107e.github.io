#ifndef PS2_KEYS_H
#define PS2_KEYS_H

#include <stdbool.h>
#include <stdint.h>

/*
 * This module manages a keymap that associates each key
 * on the PS/2 keyboard with the character it produces.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#define PS2_CODE_RELEASE 0xF0   // Scancodes used in the PS/2 protocol
#define PS2_CODE_EXTENDED 0xE0

/*
 * Type: `ps2_key_t`
 *
 * A ps2_key_t struct has three fields, `ch`, `ch_shifted`, and `key`.
 * The ch fields are the base and modified character produced by
 * the key. For example, the A key produces  'a' and 'A' and the
 * Four key produces '4' and '$'. Keys such as Tab that are unchanged
 * by shift have same char for both '\t'.
 * For keys that do not produce an ASCII character, both ch fields are
 * zero and id field is a value from non_character_id enumeration below.
 */

typedef struct {
    char ch;                // base character for key (no modifiers)
    char ch_shifted;        // char if shift applied
    uint8_t id;             // id for non-character key
} ps2_key_t;

/*
 * Enumeration for special keys that do not produce ASCII char
 *
 * Assign values >= 0x90 (out of ASCII range)
 */
enum non_character_id {
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
    PS2_KEY_ARROW_UP,
    PS2_KEY_ARROW_DOWN,
    PS2_KEY_ARROW_LEFT,
    PS2_KEY_ARROW_RIGHT
};

/*
 *  `ps2_key_for_scancode`
 *
 * Return ps2_key_t struct associated with given scancode.
 * If scancode is invalid (out of range or unmapped), returns a ps2_key_t
 * struct where all fields are zero.
 */
ps2_key_t ps2_key_for_scancode(uint8_t scancode, bool extended);

#endif
