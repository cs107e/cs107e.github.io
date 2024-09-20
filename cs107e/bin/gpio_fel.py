#!/usr/bin/python3
"""
Python script to access Mango Pi gpio peripheral using xfel

Author:  Daniel James <drjames@stanford.edu> Jan 2024
"""

import subprocess
import sys

ANSI_GRAY = "\033[90m"
ANSI_RED = "\033[31m"
ANSI_GREEN = "\033[32m"
ANSI_YELLOW = "\033[33m"
ANSI_RESET = "\033[0m"

def p_hex(num: int):
    return f"{num:#0{10}x}"

def run_xfel(args: list[str]):
    parts = ["xfel"] + args
    print(ANSI_GRAY, end="")
    print(*parts, sep=" ", )
    print(ANSI_RESET, end="")
    return subprocess.run(parts, check=True, capture_output=True)

def xfel_read(address: int):
    result = run_xfel(["read32", p_hex(address)])
    stdout_str = result.stdout.decode("utf-8")
    return int(stdout_str.strip(), 16)

def xfel_write(address: int, value):
    run_xfel(["write32", p_hex(address), p_hex(value)])        

class GpioPin:
    def __init__(self, s: str) -> None:
        try:
            pin = s.upper()
            if pin[0] == "P":
                pin = pin[1:]

            self.group = pin[0]
            self.pin_idx = int(pin[1:])

            if not self.is_valid(): raise Exception(s)

        except:
            sys.exit(f"Invalid gpio id: {s}")

    def is_valid(self):
        if   self.group == "B": return 0 <= self.pin_idx <= 12
        elif self.group == "C": return 0 <= self.pin_idx <= 7
        elif self.group == "D": return 0 <= self.pin_idx <= 22
        elif self.group == "E": return 0 <= self.pin_idx <= 17
        elif self.group == "F": return 0 <= self.pin_idx <= 6
        elif self.group == "G": return 0 <= self.pin_idx <= 18
        else: return False
    
    def cfg_base_address(self):
        if   self.group == "B": return 0x02000030
        elif self.group == "C": return 0x02000060
        elif self.group == "D": return 0x02000090
        elif self.group == "E": return 0x020000C0
        elif self.group == "F": return 0x020000F0
        elif self.group == "G": return 0x02000120
        else: return None
    
    def data_base_address(self):
        if   self.group == "B": return 0x02000040
        elif self.group == "C": return 0x02000070
        elif self.group == "D": return 0x020000A0
        elif self.group == "E": return 0x020000D0
        elif self.group == "F": return 0x02000100
        elif self.group == "G": return 0x02000130
        else: return None
    
    def read_fn(self):
        addr = self.cfg_base_address() + (self.pin_idx // 8) * 4
        shift = (self.pin_idx % 8) * 4
        return (xfel_read(addr) >> shift) & 0xF

    def write_fn(self, fn: int):
        if fn < 0 or fn > 15: raise Exception(f"Invalid function {fn}.")

        addr = self.cfg_base_address() + (self.pin_idx // 8) * 4
        shift = (self.pin_idx % 8) * 4
        curr = xfel_read(addr)
        curr &= ~(0xF << shift) # unset the bits for our pin
        curr |= fn << shift     # and set the bits for the function
        xfel_write(addr, curr)
    
    def read_val(self):
        addr = self.data_base_address()
        return (xfel_read(addr) >> self.pin_idx) & 1 == 1

    def write_val(self, val: bool):
        addr = self.data_base_address()
        curr = xfel_read(addr)
        curr &= ~(1 << self.pin_idx)
        curr |= (1 if val else 0) << self.pin_idx
        xfel_write(addr, curr)

    def __str__(self):
        return f"P{self.group}{self.pin_idx}"


def parse_state(state: str):
    if state in ["high", "true", "1"]: return True
    elif state in ["low", "false", "0"]: return False
    else: return None


def pretty_print_fn(fn: int):
    if fn == 0: print("0 - Input")
    elif fn == 1: print("1 - Output")
    elif 2 <= fn <= 8: print(f"{fn} - Alt{fn}")
    elif 9 <= fn <= 13: print(f"{fn} - Reserved")
    elif fn == 14: print("14 - Interrupt")
    elif fn == 15: print("15 - Disabled")
    else: print(f"Function {fn}? That should be possible!")


def parse_fn(fn: str):
    if fn == "in" or fn == "input": return 0
    elif fn == "out" or fn == "output": return 1
    elif fn == "alt2": return 2
    elif fn == "alt3": return 3
    elif fn == "alt4": return 4
    elif fn == "alt5": return 5
    elif fn == "alt6": return 6
    elif fn == "alt7": return 7
    elif fn == "alt8": return 8
    elif fn == "interrupt": return 14
    elif fn == "disabled" or fn == "disable": return 15
    else:
        num = int(fn)
        if 0 <= num <= 15:
            return num
        else:
            return None


def show_pin(pin):
    #pin = GpioPin(id)
    print("----")
    fn = pin.read_fn()
    if fn in [0, 1]:
        val = pin.read_val()
        valstr = f" val: {'1 - High' if val else '0 - Low'},"
    else:
        valstr = ''
    print(f"[{pin}]{valstr} fn: ", end='')
    pretty_print_fn(fn)


def help_message():
    print(f"A tool for interacting with the GPIO pins on the mango pi")
    print(f"")
    print(f"{ANSI_YELLOW}Usage:{ANSI_RESET} gpio_fel.py [COMMAND]")
    print(f"")
    print(f"{ANSI_YELLOW}Commands:{ANSI_RESET}")
    print(f"   {ANSI_GREEN} help              {ANSI_RESET}   - Print this help message")
    print(f"   {ANSI_GREEN} read <pin>        {ANSI_RESET}   - Read the current state of the pin")
    print(f"   {ANSI_GREEN} write <pin> <val> {ANSI_RESET}   - Set the state of the pin (high, low, true, false, 0, 1)")
    print(f"   {ANSI_GREEN} fn get <pin>      {ANSI_RESET}   - Get the function of the pin")
    print(f"   {ANSI_GREEN} fn set <pin> <fn> {ANSI_RESET}   - Set the function of pin pin (0-15, in, out, alt2-8, interrupt, disabled)")
    print(f"   {ANSI_GREEN} show <pin> <pin>...{ANSI_RESET}  - Show function and state of pin(s) (if no argument, show all pins)")

def main():
    argv = sys.argv

    if len(argv) == 1 or argv[1] == "help" or argv[1] == "--help" or argv[1] == "-h":
        help_message()
        exit()

    if argv[1] == "read":
        pin = GpioPin(argv[2])
        print("1 - High" if pin.read_val() else "0 - Low")

    elif argv[1] == "write":
        pin = GpioPin(argv[2])
        state = parse_state(argv[3])
        pin.write_val(state)

    elif argv[1] == "fn" and argv[2] == "get":
        pin = GpioPin(argv[3])        
        pretty_print_fn(pin.read_fn())

    elif argv[1] == "fn" and argv[2] == "set":
        pin = GpioPin(argv[3])
        fn = parse_fn(argv[4])
        pin.write_fn(fn)

    elif argv[1] == "show":
        if (len(argv) > 2):
            ids = argv[2:]
        else:
            groups = {'B':12, 'C':7, 'D':22, 'E':17, 'F':6, 'G':18}
            ids =  [f"P{g}{i}" for g in groups for i in range(0, groups[g]+1)]
        for p in [GpioPin(id) for id in ids]:
            show_pin(p)

    else:
        help_message()


if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError:
        print(f"{ANSI_RED}There was an error with xfel. Are you sure the MangoPi is connected?{ANSI_RESET}")
        exit(1)
