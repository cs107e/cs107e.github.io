#!/usr/bin/python3
"""
Quick & dirty python script to print ASCII art version of Mango Pi pinout.

View help message with `pinout.py -h`.

Author: Julie Zelenski <zelenski@cs.stanford.edu> Dec 2023
Updated by Daniel James <drjames@stanford.edu> Jan 2024
Documented, Updated by Marcus Alagar <marcusal@stanford.edu> Jan 2025
"""

import argparse, os, re, sys

def wrap(text, *colors):
    """Wraps given text in the specified color codes

    Takes string `text` and variable-length tuple of string `colors`
    Returns a copy of `text` wrapped in ANSI escape codes to display
    in specified colors and reset to normal after.
    """

    # With help of https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    COLORS = {
        'normal': 0,
        'black': 100, # In ANSI, is technically bright black
        'black_fg': 90,
        'red': 41,
        'green': 42,
        'yellow': 43,
        'cyan': 46,
        'magenta_fg': 35,
        'white': 107, # In ANSI, is technically bright white
        'white_fg': 97
    }
    if ARGS.no_color: # if colors disabled, return text as-is
        return text
    else:
        ansi_escape = lambda name: f"\033[{COLORS[name]}m"
        return ''.join((ansi_escape(c) for c in colors)) + text + ansi_escape('normal')

PINS = {
    1:  ('3V3', 'yellow'),   2:  ('5V',  'red'),
    3:  ('PG13','green'),    4:  ('5V',  'red'),
    5:  ('PG12','green'),    6:  ('GND', 'black'),
    7:  ('PB7', 'green'),    8:  ('PB8', 'green'),
    9:  ('GND', 'black'),    10: ('PB9', 'green'),
    11: ('PD21','green'),    12: ('PB5', 'green'),
    13: ('PD22','green'),    14: ('GND', 'black'),
    15: ('PB0', 'green'),    16: ('PB1', 'green'),
    17: ('3V3', 'yellow'),   18: ('PD14','green'),
    19: ('PD12','green'),    20: ('GND', 'black'),
    21: ('PD13','green'),    22: ('PC1', 'green'),
    23: ('PD11','green'),    24: ('PD10','green'),
    25: ('GND', 'black'),    26: ('PD15','green'),
    27: ('PE17','cyan'),     28: ('PE16','cyan'),
    29: ('PB10','green'),    30: ('GND', 'black'),
    31: ('PB11','green'),    32: ('PC0', 'green'),
    33: ('PB12','green'),    34: ('GND', 'black'),
    35: ('PB6', 'green'),    36: ('PB2', 'green'),
    37: ('PD17','green'),    38: ('PB3', 'green'),
    39: ('GND', 'black'),    40: ('PB4', 'green')
}

LABELS = {8:'UART TX', 10:'UART RX'}

def pin_color(pin_num):
    return PINS[pin_num][1]

def pinbox(pin_num, target_set):
    """ Returns formatted string for pin with `pin_num`
        if not in target_set uses plain dot else colored box or @
    """
    if pin_num == 1: return '1'
    elif pin_num not in target_set: return '·'
    elif ARGS.no_color: return '@'
    else: return wrap('·', pin_color(pin_num))

def print_board(target_set):
    """Prints ASCII board. Use color settings in `PINS` to beautify header.
    """

    BOARD = [
        "                                             ",
        "    │ OTG │  │ USB │            │ HDMI │     ",
        " O──┤     ├──┤     ├────────────┤ mini ├───O ",
        " │                  ┌──────┐      ┌─────┐  │ ",
        " │     " + wrap('Mango Pi','magenta_fg') + "     │  D1  │      │ µsd │  │ ",
        " │     " + wrap(' MQ-Pro ','magenta_fg') + "     │      │      │     │  │ ",
        " │                  └──────┘      └─────┘  │ ",
        " │                                         │ ",
        " │ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ │ ",
        " │ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ │ ",
        " │ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ │ ",
        " O─────────────────────────────────────────O ",
    ]

    # Apply pinbox to color header pins ('@') according to PINS
    # First row (odd): iterate odd-numbered pins 39-1 in reverse
    # Second row (even): iterate even-numbered pins 40-2 in reverse
    pins = (39,-1,-2)
    for row in BOARD:
        if '@' in row:          # if row is header
            pin_iter = iter(range(*pins))
            row = ''.join((pinbox(next(pin_iter), target_set) if ch == '@' else ch) for ch in row)
            pins = (40,0,-2) # set up for subsequent row
        print(row)

def pin_info_formatted(pin_num, target_set):
    """Formats label and pin strings

    Takes `pin_num` and `target_set`. Returns tuple (label, pin)
    of label string and formatted pin number string.
    Dot label if not in target set, otherwise label from PINS
    Pin number format pin color for background, white foreground text.
    """
    if pin_num not in target_set:
        label = ''
        id = '·'
    else:
        label = LABELS[pin_num] if pin_num in LABELS else ''
        id = PINS[pin_num][0]
    number_str  = wrap(f"{pin_num:3} ", "white_fg", pin_color(pin_num))
    return (label, id, number_str)

def print_header_table(target_set):
    """Prints colored table of headers

    Takes a set of pin numbers `target_set`. Prints headers,
    pin backgrounds use color from PINS, labels are displayed for all
    pins in target set.
    """
    for pin_num in range(1, 41, 2):
        l_label,l_id,l_pin_no = pin_info_formatted(pin_num, target_set)
        r_label,r_id,r_pin_no = pin_info_formatted(pin_num + 1, target_set)
        print(f"{l_label:>10}   {l_id:>4} {l_pin_no} │ {r_pin_no} {r_id:<4}   {r_label:<10}")

def as_pin_number(id, quiet=False):
    """Convert string (either label, gpio id, or pin number) to pin number(s) (GND/PWR have multi entries)
    Report parse error if label/id/number not valid
    """
    if id.isnumeric():
        nums = {int(id)}
    else:
        nums = set(pin_num for pin_num,pair in PINS.items() if pair[0] == id.upper())
    if not quiet and not (nums & PINS.keys()): # empty intersect w/ pins on header
        parser.error(f"Header has no pin '{id}'. Id can be pin label (PB3, GND), pin range (PB2-4), pin number (1 to 40), larson, or clock.")
    else:
        return nums

def parse_range_specifier(s):
    """Takes string arg and attempts to parse as range specifier
    If match regex pattern, ids are those in start-end inclusive
    Returns list of ids in range (or None if range is not valid)
    """
    # https://www.w3schools.com/python/python_regex.asp
    # https://docs.python.org/3/library/re.html
    # All hail https://regexr.com/
    match = re.match(r'P([B-G])(\d\d?)-(\d\d?)', s, re.IGNORECASE)
    if match is not None:
        letter = match.group(1).upper()
        start,end = int(match.group(2)), int(match.group(3))
        return [f"P{letter}{i}"for i in range(start, end + 1)]
    else:
        return None

def process_ids(ids):
    """Takes id arguments and process into target set of pin numbers
    """
    pin_set = set()
    for arg in ids:
        range_ids = []
        labels = {}
        if arg == 'larson':
            range_ids = parse_range_specifier('PB0-7')
        elif arg == 'clock':
            labels = {'PD17':'SegA','PB6':'SegB','PB12':'SegC','PB11':'SegD','PB10':'SegE','PD11':'SegF','PD13':'SegG',
                       'PB4':'Dig1','PB3':'Dig2','PB2':'Dig3','PC0':'Dig4',
                       'PD12':'Button'}
        elif arg == 'keyboard':
            labels = {'PG12':'Key CLK','PB7':'Key DATA'}
        elif arg == 'mouse':
            labels = {'PD21':'Mouse CLK','PD22':'Mouse DATA'}
        elif arg == 'ps2':
            # loopback connections for ps2 simulator used in grading
            labels = {'PD11':'Device CLK','PD10':'Device DATA','PD13':'Host CLK','PC1':'Host DATA'}
        elif arg == 'pwm':
            labels = {'PB12':'PWM0','PB6':'PWM1','PB11':'PWM2','PB0':'PWM3','PB1':'PWM4','PB8':'PWM5','PB9':'PWM6','PB10':'PWM7'}
        elif arg == 'spi':
            labels = {'PD11':'SCLK','PD12':'MOSI','PD13':'MISO','PD10':'CS'}
        elif arg == 'i2c':
            labels = {'PG13':'SDA','PG12':'SCL'}
        elif arg == 'i2s':
            labels = {'PB5':'CLK','PB6':'FS','PB3':'Din','PB4':'Dout'}
        elif parse_range_specifier(arg):
            range_ids = parse_range_specifier(arg)
        else:
            pin_set |= as_pin_number(arg)
            continue

        # process list/ranges
        for (id,label) in labels.items():
            added = as_pin_number(id)
            pin_set |= added
            for n in added: LABELS[n] = label
        for id in range_ids:
            pin_set |= as_pin_number(id, quiet=True) # ignore if gaps in range
    return pin_set

if __name__ == "__main__":
    # Use argparse to handle command-line arguments
    parser = argparse.ArgumentParser(description=f"This script displays pinout of the Mango Pi MQ-Pro.")
    nc_env = os.environ.get("NO_COLOR")  # respect NO_COLOR environment setting see https://no-color.org
    nc_default = nc_env is not None and nc_env != ""
    parser.add_argument('ids', help="which pins to highlight, can specify as single pin PD18, range PB0-5, pin number 1 to 40, or label such as GND", nargs='*', action="extend")
    parser.add_argument('-n', help="disable colors", action="store_true", dest="no_color", default=nc_default)
    ARGS = parser.parse_args()
    target_set = process_ids(ARGS.ids)
    if not target_set: target_set = PINS.keys() # if no selected ids, all pins in target set

    print_board(target_set)
    print()
    print_header_table(target_set)
