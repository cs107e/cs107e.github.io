/*
 * Keymap for PS/2 scancode set 2
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */

#include "ps2_keys.h"

#define LETTER(lowerch)          {.ch=lowerch, .ch_shifted=lowerch-'a'+'A', .id=0}
#define SYMBOL(normal, shifted)  {.ch=normal,  .ch_shifted=shifted,         .id=0}
#define SPECIAL(key)             {.ch=0,       .ch_shifted=0,               .id=key}

static const ps2_key_t keymap[] = {
    // Row 1: esc, function keys
    [0x76] = SPECIAL(PS2_KEY_ESC),
    [0x05] = SPECIAL(PS2_KEY_F1),
    [0x06] = SPECIAL(PS2_KEY_F2),
    [0x04] = SPECIAL(PS2_KEY_F3),
    [0x0C] = SPECIAL(PS2_KEY_F4),
    [0x03] = SPECIAL(PS2_KEY_F5),
    [0x0B] = SPECIAL(PS2_KEY_F6),
    [0x83] = SPECIAL(PS2_KEY_F7),
    [0x0A] = SPECIAL(PS2_KEY_F8),
    [0x01] = SPECIAL(PS2_KEY_F9),
    [0x09] = SPECIAL(PS2_KEY_F10),
    [0x78] = SPECIAL(PS2_KEY_F11),
    [0x07] = SPECIAL(PS2_KEY_F12),

    // Row 2: nums, backspace
    [0x0E] = SYMBOL('`', '~'),
    [0x16] = SYMBOL('1', '!'),
    [0x1E] = SYMBOL('2', '@'),
    [0x26] = SYMBOL('3', '#'),
    [0x25] = SYMBOL('4', '$'),
    [0x2E] = SYMBOL('5', '%'),
    [0x36] = SYMBOL('6', '^'),
    [0x3D] = SYMBOL('7', '&'),
    [0x3E] = SYMBOL('8', '*'),
    [0x46] = SYMBOL('9', '('),
    [0x45] = SYMBOL('0', ')'),
    [0x4E] = SYMBOL('-', '_'),
    [0x55] = SYMBOL('=', '+'),
    [0x66] = SYMBOL('\b', '\b'),

    // Row 3: tab, qwerty
    [0x0D] = SYMBOL('\t', '\t'),
    [0x15] = LETTER('q'),
    [0x1D] = LETTER('w'),
    [0x24] = LETTER('e'),
    [0x2D] = LETTER('r'),
    [0x2C] = LETTER('t'),
    [0x35] = LETTER('y'),
    [0x3C] = LETTER('u'),
    [0x43] = LETTER('i'),
    [0x44] = LETTER('o'),
    [0x4D] = LETTER('p'),
    [0x54] = SYMBOL('[', '{'),
    [0x5B] = SYMBOL(']', '}'),
    [0x5D] = SYMBOL('\\', '|'),

    // Row 4: caps lock, asdf, return
    [0x58] = SPECIAL(PS2_KEY_CAPS_LOCK),
    [0x1C] = LETTER('a'),
    [0x1B] = LETTER('s'),
    [0x23] = LETTER('d'),
    [0x2B] = LETTER('f'),
    [0x34] = LETTER('g'),
    [0x33] = LETTER('h'),
    [0x3B] = LETTER('j'),
    [0x42] = LETTER('k'),
    [0x4B] = LETTER('l'),
    [0x4C] = SYMBOL(';', ':'),
    [0x52] = SYMBOL('\'', '"'),
    [0x5A] = SYMBOL('\n', '\n'),

    // Row 5: shift, zxcv
    [0x12] = SPECIAL(PS2_KEY_SHIFT),
    [0x1A] = LETTER('z'),
    [0x22] = LETTER('x'),
    [0x21] = LETTER('c'),
    [0x2A] = LETTER('v'),
    [0x32] = LETTER('b'),
    [0x31] = LETTER('n'),
    [0x3A] = LETTER('m'),
    [0x41] = SYMBOL(',', '<'),
    [0x49] = SYMBOL('.', '>'),
    [0x4A] = SYMBOL('/', '?'),
    [0x59] = SPECIAL(PS2_KEY_SHIFT),

    // Row 6: modifiers, space, arrows
    [0x14] = SPECIAL(PS2_KEY_CTRL),
    [0x11] = SPECIAL(PS2_KEY_ALT),
    [0x29] = SYMBOL(' ', ' '),
    [0x6B] = SPECIAL(PS2_KEY_ARROW_LEFT),
    [0x75] = SPECIAL(PS2_KEY_ARROW_UP),
    [0x72] = SPECIAL(PS2_KEY_ARROW_DOWN),
    [0x74] = SPECIAL(PS2_KEY_ARROW_RIGHT),
};

static const ps2_key_t extended_keymap[] = {
    [0x14] = SPECIAL(PS2_KEY_CTRL),
    [0x11] = SPECIAL(PS2_KEY_ALT),
    [0x6B] = SPECIAL(PS2_KEY_ARROW_LEFT),
    [0x75] = SPECIAL(PS2_KEY_ARROW_UP),
    [0x72] = SPECIAL(PS2_KEY_ARROW_DOWN),
    [0x74] = SPECIAL(PS2_KEY_ARROW_RIGHT),
};

ps2_key_t ps2_key_for_scancode(uint8_t scancode, bool extended) {
    static const int n = sizeof(keymap)/sizeof(*keymap);
    static const int n_ext = sizeof(extended_keymap)/sizeof(*extended_keymap);
    static const ps2_key_t KEY_NONE = {0, 0, 0};

    if ((extended && scancode >= n_ext) || (!extended && scancode >= n)) {
        return KEY_NONE;
    }
    return (extended ? extended_keymap[scancode] : keymap[scancode]);
}
