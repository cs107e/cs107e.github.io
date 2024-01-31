#!/usr/bin/env python3

"""
Should work with:
- Python 3
- macOS, linux, WSL/ubuntu on Windows
Dependencies:

    # pip3 install {pyserial}

Defaults to CP2102 device that matches specified vendor/product id.
See below for where to set these IDs to match unit in use.
"""

from __future__ import print_function
import platform, re, serial, subprocess, sys
from serial.tools import list_ports

# See VERSION numbering above
VERSION = "2.3"

# Set the vendor and product ID of the serial unit.
# The CP2102 units used during years 2018 - 2022 all have
# vendor ID 0x10C4 and product ID 0xEA60.
SERIAL_VID = "10C4"
SERIAL_PID = "EA60"

# From https://stackoverflow.com/questions/287871/print-in-terminal-with-colors-using-python
# Plus Julie's suggestion to push bold and color together.
class bcolors:
    RED = '\033[31m'
    BLUE = '\033[34m'
    GREEN = '\033[32m'
    BOLD = '\033[1m'
    OKBLUE = BOLD + BLUE
    OKGREEN = BOLD + GREEN
    FAILRED = BOLD + RED
    ENDC = '\033[0m'

# if stdout is not a terminal, don't output color codes, set them to empty string
if not sys.stdout.isatty():
    [setattr(bcolors, v,'') for v in vars(bcolors) if not v.startswith('_')]

# Added code to communicate exit status
class exitcode:
    OK = 0
    SERIAL_ERROR = 5

def error(shortmsg, explanation="", code=1):
    sys.stderr.write(f"\n{bcolors.FAILRED}{shortmsg}{bcolors.ENDC}")
    # only output long-winded explanation at terminal
    if sys.stderr.isatty(): sys.stderr.write(f"\n\n{explanation}\n")
    sys.exit(code)

# We used to just have a preset list --
# /dev/tty.SLAB_USBtoUART for macOS + Silicon Labs driver,
# /dev/cu.usbserial for Prolific,
# /dev/ttyUSB0 for Linux, etc.
# Instead find port by matching device with vendor/product id of our CP2102 board
def find_serial_port():
    portname = None
    try:
        # pyserial 2.6 in the VM has a bug where grep is case-sensitive.
        # It also requires us to use [0] instead of .device on the result
        # to get the serial device path.
        portname = next(list_ports.grep(r'(?i)VID:PID=%s:%s' % (SERIAL_VID, SERIAL_PID)))[0]
    except StopIteration:
        pass

    # Device IDs aren't available in WSL. In this case, we do the search through PowerShell and map the output to the Linux device name
    if portname is None and platform.system() == "Linux" and "Microsoft" in platform.release():
        ps_cmd = ['powershell.exe', '(Get-WmiObject -query "SELECT * FROM Win32_PnPEntity" | ' +
                  'Where-Object {$_.DeviceID -like "*VID_%s&PID_%s*"}).Name' % (SERIAL_VID, SERIAL_PID)]
        output = subprocess.check_output(ps_cmd).strip()
        match = re.search(rb'\(COM(\d+)\)', output)
        if match:
            portname = '/dev/ttyS' + match.group(1).decode('utf-8')

    if portname is not None:
        return portname
    else:
        error("Could not find CP2102 serial device.",
              "I looked through the serial devices on this computer, and did not\n"
              "find a device associated with a CP2102 USB-to-serial adapter. Is\n"
              "your Pi plugged in?",
              code=exitcode.SERIAL_ERROR)

if __name__ == "__main__":
    portname = find_serial_port()
    print('Found serial device:', bcolors.OKBLUE + portname + bcolors.ENDC)

    sys.exit(exitcode.OK)  #
