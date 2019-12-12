#ifndef PS2_H
#define PS2_H

/* This module declares constants for interacting with a PS/2
 * keyboard, including a useful array that serves as a lookup table
 * to access information about the keys and their associated
 * characters.
 *
 * Each entry in the array corresponds to one key on the keyboard.
 * Pressing a key generates a unique PS/2 scancode. The array is organized
 * in order of scancode. A scancode can be used as an index to
 * access the ASCII character associated with that key. The `other_ch`
 * is the shifted char or 0 if this key has no shifted char. For a key 
 * that is not associated with a printable character, its associated
 * char will  be represented by a code from the ps2_codes enumeration below.
 *
 * You will use this interface in assignment 5 to implement a keyboard
 * driver.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Date:   February 2018
 */ 

typedef struct {
    unsigned char ch;
    unsigned char other_ch;
} ps2_key_t;

extern ps2_key_t const ps2_keys[];

/* Since regular chars have ASCII values that are all <= 0x7F,
 * we assign codes >= 0x90 to the non-char keys.
 */
enum ps2_codes {
    PS2_KEY_NONE = 0,
    PS2_CODE_RELEASE = 0xF0,
    PS2_CODE_EXTENDED = 0xE0,
    PS2_KEY_SHIFT = 0x90,   
    PS2_KEY_ALT,    // values assigned in increasing sequence from here
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