#!/usr/bin/python3
"""
Python script to access Mango Pi gpio peripheral using xfel

Author:  Daniel James <drjames@stanford.edu> Jan 2024
Updated: Julie Zelenski, Winter quarter 2026
"""

import subprocess,sys

COLORS = {'normal':'\033[0m', 'red':'\033[31m', 'green':'\033[32m', 'yellow':'\033[33m',
          'bold':'\033[1m', 'faint':'\033[2m', 'underline':'\033[4m'}
def wrap_fn(color): return lambda s: COLORS[color] + str(s) + COLORS['normal']
for color in COLORS: setattr(sys.modules[__name__], color, wrap_fn(color))

def xfel_command(cmd, *args):
    shell_cmd = ['xfel', cmd] + [f"{a:#0{10}x}" for a in args]
    print(faint(' '.join(shell_cmd)))
    try:
        result = subprocess.run(shell_cmd, check=True, capture_output=True)
        return result.stdout.decode('utf-8').strip()
    except Exception:
        print(red(f"Error calling xfel. Are you sure the MangoPi is connected?"))
        sys.exit(1)

def xfel_read(address: int) -> int:
    out = xfel_command('read32', address)
    return int(out, 16)

def xfel_write(address: int, value):
    xfel_command('write32', address, value)

class GpioPin:

    GROUPS = {# (max id, cfg base addr)
        'B': (12, 0x02000030),
        'C': ( 7, 0x02000060),
        'D': (22, 0x02000090),
        'E': (17, 0x020000C0),
        'F': ( 6, 0x020000F0),
        'G': (18, 0x02000120),
    }

    def __init__(self, s: str) -> None:
        try:
            idstr = s.upper()
            if idstr[0] == 'P': idstr = idstr[1:]
            self.group = idstr[0]
            self.index = int(idstr[1:])
            (max, cfg_base_addr) = GpioPin.GROUPS[self.group]
            if not (0 <= self.index <= max): raise Exception()
            self.cfg_addr = cfg_base_addr + (self.index // 8) * 4
            self.data_addr = cfg_base_addr + 0x10
            self.cfg_shift = (self.index % 8) * 4

        except (KeyError, ValueError, IndexError):
            sys.exit(red(f"Error: invalid gpio id '{s}'"))

    def read_fn(self):
        return (xfel_read(self.cfg_addr) >> self.cfg_shift) & 0xF

    def write_fn(self, fn: int):
        xfel_write(self.cfg_addr, (xfel_read(self.cfg_addr) & ~(0xF << self.cfg_shift)) | (fn << self.cfg_shift))

    def read_val(self):
        return (xfel_read(self.data_addr) >> self.index) & 0x1

    def write_val(self, val: bool):
        curr = xfel_read(self.data_addr)
        shift = 1 << self.index
        xfel_write(self.data_addr, (curr | shift) if val else (curr & ~shift))

    def __str__(self):
        return f"P{self.group}{self.index}"

    def all_pins():
        return [f"P{g}{i}" for (g,(n,_)) in GpioPin.GROUPS.items() for i in range(0, n+1)]


def fn_name(num: int) -> str:
    FN_NAMES = {0:'input',1:'output',14:'interrupt',15:'disabled'}
    if num in FN_NAMES: return FN_NAMES[num]
    if 2 <= num <= 8: return f"alt{num}"
    if 9 <= num <= 13: return "reserved"
    return None

def parse_fn(s: str) -> int:
    try:
        as_int = int(s)
        if fn_name(as_int) is not None: return as_int
    except:
        for i in range(0, 16):
            if s.lower() == fn_name(i).lower(): return i
    sys.exit(red(f"Error: invalid fn '{s}'"))

def parse_state(s: str) -> bool:
    s = s.lower()
    if s in ['on', 'high', 'true', '1']: return True
    elif s in ['off', 'low', 'false', '0']: return False
    sys.exit(red(f"Error: invalid state '{s}'"))

def show_pin(pin, *args):
    print('----')
    valstr = fnstr = ''
    if 'fn' in args or 'all' in args:
        fn = pin.read_fn()
        fnstr = f"fn: {fn} ({fn_name(fn).capitalize()})"
    if ('val' in args or ('all' in args and fn != 15)):
        valstr = f"val: {'1 (High)' if pin.read_val() else '0 (Low)'}"
    print(f"[{pin}] {valstr} {fnstr}")

def show_usage():
    commands = [('read', ['pin'], 'Read pin state'),
                ('write', ['pin','val'], 'Set pin state (val can be high,1,true,low,0,false)'),
                ('getfn', ['pin'], 'Get pin function'),
                ('setfn', ['pin', 'fn'], 'Set pin function (fn can be 0-15,input,output,alt2-8,interrupt,disabled)'),
                ('show', ['[pin]', '...'], 'Show function and state of one or more pins (if no argument, show all pins)')]

    print(f"A tool for interacting with the GPIO pins on the Mango Pi\n")
    print(f"{yellow('Usage:')} gpio_fel.py {underline('subcommand')}\n")
    print(f"{yellow('Subcommands:')}")
    for (cmd,args,desc) in commands:
        print(f"    {green(cmd)} {(' '.join(underline(a) for a in args))}    \t- {desc}")


if __name__ == '__main__':

    if len(sys.argv) == 1 or sys.argv[1] == '--help' or sys.argv[1] == '-h':
        show_usage()
        sys.exit()

    try:
        subcommand = sys.argv[1]
        args = sys.argv[2:]
        if subcommand == 'read':
            show_pin(GpioPin(args[0]), 'val')

        elif subcommand == 'write':
            pin = GpioPin(args[0])
            pin.write_val(parse_state(args[1]))

        elif subcommand == 'getfn':
            show_pin(GpioPin(args[0]), 'fn')

        elif subcommand == 'setfn':
            pin = GpioPin(args[0])
            pin.write_fn(parse_fn(args[1]))

        elif subcommand == 'show':
            ids = args if len(args) > 0 else GpioPin.all_pins()
            for p in [GpioPin(id) for id in ids]:
                show_pin(p, 'all')

        else:
            print(bold(red(f"Error: gpio_fel.py: no such subcommand '{subcommand}'\n")))
            show_usage()

    except IndexError:
        print(red(bold(f"Error: gpio_fel.py {subcommand} {' '.join(args)}  ")), red("missing required argument(s)"))
