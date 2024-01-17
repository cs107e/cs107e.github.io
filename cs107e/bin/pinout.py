#!/usr/bin/python3
"""
Quick & dirty python script to print ASCII art version of Mango Pi pinout

Author: Julie Zelenski <zelenski@cs.stanford.edu> Dec 2023
Updated by Daniel James <drjames@stanford.edu> Jan 2024
"""
import sys

USE_COLORS = True
def wrap(text, *colors):
    COLORS = {
        'normal': 0,
        'black': 100,
        'black_fg': 90,
        'red': 41,
        'red_fg': 31,
        'green': 42,
        'green_fg': 32,
        'yellow': 43,
        'yellow_fg': 33,
        'blue': 44,
        'blue_fg': 34,
        'magenta_fg': 35,
        'white_fg': 97
        }
    ansi_escape = lambda name: f"\033[{COLORS[name]}m" if USE_COLORS else ''
    return ''.join((ansi_escape(c) for c in colors)) + text + ansi_escape('normal')

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

HEADERS = [
    [ ("3V3",'yellow'), ("5V",'red') ],
    [ ("PG13",'green'), ("5V",'red') ],
    [ ("PG12",'green'), ("GND",'black') ],
    [ ("PB7",'green'),  ("PB8 (TX)",'green') ],
    [ ("GND",'black'),  ("PB9 (RX)",'green') ],
    [ ("PD21",'green'), ("PB5",'green') ],
    [ ("PD22",'green'), ("GND",'black') ],
    [ ("PB0",'green'),  ("PB1",'green') ],
    [ ("3V3",'yellow'), ("PD14",'green') ],
    [ ("MOSI",'green'), ("GND",'black') ],
    [ ("MISO",'green'), ("PC1",'green') ],
    [ ("SCLK",'green'), ("CS0",'green') ],
    [ ("GND",'black'),  ("PD15",'green') ],
    [ ("PE17",'blue'),  ("PE16",'blue') ],
    [ ("PB10",'green'), ("GND",'black') ],
    [ ("PB11",'green'), ("PC0",'green') ],
    [ ("PB12",'green'), ("GND",'black') ],
    [ ("PB6",'green'),  ("PB2",'green') ],
    [ ("PD17",'green'), ("PB3",'green') ],
    [ ("GND",'black'),  ("PB4",'green') ],
]

def colored_header(str, colors):
    return ''.join((wrap(' ', next(colors))) if ch == '@' else ch for ch in str)

def print_board():
    indexes = iter(range(0, 2))
    for row in BOARD:
        if '@' in row:
            header_index = next(indexes)
            row = colored_header(row, (c[header_index][1] for c in reversed(HEADERS)))
        print(row)

def print_header_table():
    pin_numbers = iter(range(1, 41))
    pin = lambda color: wrap("▐", color+"_fg") + wrap(f"{next(pin_numbers):2}","white_fg", color) + wrap("▌", color + "_fg")
    for pair in HEADERS:
        l_label,l_pin = pair[0][0], pin(pair[0][1])
        r_label,r_pin = pair[1][0], pin(pair[1][1])
        print(f"  {l_label:4} {l_pin} │ {r_pin} {r_label}")

if __name__ == '__main__':
    USE_COLORS = not(len(sys.argv) >= 2 and sys.argv[1].startswith('-n'))
    print_board()
    print()
    print_header_table()
