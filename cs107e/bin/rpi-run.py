#!/usr/bin/env python3

"""
-------- VERSION numbering ----------
        First rpi-install.py by Pat Hanrahan, 2015-16
0.8     Edited by Omar Rizwan 2017-04-23.
0.91    Updated by Julie Zelenski Winter 2018
1.0     WSL support added Nov 2018 by mchang
1.1     Updated Winter 2020 (python3, wsl)
2.0     Bump major version number when rename to rpi-run.py Winter 2021
2.1     Add exit code for integration with auto-test Spring 2022
2.2     Removed awkward timeout handling, better for lsi to handle
2.3     Added handling for receive termination code from Pi (assert/abort)
-----------------------------------

This bootloader client is used to upload binary image to execute on
Raspberry Pi.

Communicates over serial port using xmodem protocol.

Should work with:
- Python 3
- macOS, linux, WSL/ubuntu on Windows
- on-Pi bootloader distributed by cs107e

Dependencies:

    # pip3 install {pyserial,xmodem}

Defaults to CP2102 device that matches specified vendor/product id.
See below for where to set these IDs to match unit in use.
"""

from __future__ import print_function
import argparse, errno, logging, os, platform, re, select, serial, subprocess, sys, time
from serial.tools import list_ports
from xmodem import XMODEM

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
    EOT = 0
    USAGE = 2
    USER_CANCEL = 3
    BOOTLOAD_FAIL = 4
    SERIAL_ERROR = 5
    PI_TERMINATE = 15   # SIGTERM

def error(shortmsg, explanation="", code=1):
    sys.stderr.write(f"\n{args.exename}: {bcolors.FAILRED}{shortmsg}{bcolors.ENDC}")
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
    parser = argparse.ArgumentParser(description=f"This script sends a binary file to the Raspberry Pi bootloader. Version {VERSION}")

    parser.add_argument('file', help="binary file to upload", type=argparse.FileType('rb'), nargs='?')
    parser.add_argument('-d', help="serial device (e.g. /dev/ttyUSB0), otherwise search for CP2102 device", type=str, dest="port", metavar="device")
    parser.add_argument('-v', help="verbose logging of serial activity", action="store_true")
    parser.add_argument('-q', help=argparse.SUPPRESS, action="store_true")

    after = parser.add_mutually_exclusive_group()
    after.add_argument('-p', help="open serial connection and print output received from Pi",
                       action="store_true")
    after.add_argument('-s', help="start `screen` to send/receive with Pi",
                       action="store_true")

    args = parser.parse_args()
    args.exename = os.path.basename(sys.argv[0])

    def printq(*pos_args, **kwargs):
        if not args.q:
            print(*pos_args, **kwargs)
            sys.stdout.flush()

    def printv(*pos_args, **kwargs):
        if args.v:
            print(*pos_args, **kwargs)
            sys.stdout.flush()

    logging.getLogger().addHandler(logging.StreamHandler())
    if args.v: logging.getLogger().setLevel(logging.DEBUG)

    if args.port:
        portname = args.port
        printq('Using serial device:', bcolors.OKBLUE + portname + bcolors.ENDC)
    else:
        portname = find_serial_port()
        printq('Found serial device:', bcolors.OKBLUE + portname + bcolors.ENDC)

    try:
        # if existing rpi screen is hanging onto port, terminate it now
        if os.system("screen -S rpi -X select . 2>&1 >/dev/null") == 0:
            os.system("screen -S rpi -X quit 2>&1 >/dev/null")
            printq("(had to close active `screen` on port first)")
            time.sleep(1)
    except:
        pass

    try:
        # timeout set at creation of Serial will be used as default for both read/write
        port = serial.Serial(port=portname, baudrate=115200, timeout=2, exclusive=True)

        # Opening the port seems to always pull DTR low, so go ahead
        # and perform complete reset sequence no matter what. If DTR
        # unconnected, behaves as no-op.
        printv("Toggling DTR pin to reset Pi: low... ", end='')
        port.dtr = True  # Pull DTR pin low.
        time.sleep(0.2)  # Wait for Pi to reset.
        printv("high. Waiting for Pi to boot... ", end='')
        port.dtr = False  # Pull DTR pin high.
        time.sleep(1)     # Wait for Pi to boot.
        printv("Done.")

    except (OSError, serial.serialutil.SerialException) as e:
        if e.errno in [errno.EBUSY, errno.EWOULDBLOCK]:
            error(f"The serial device `{portname}` is busy.",
                    "Do you have a `screen` or `rpi-run.py` currently active on that device?",
                    code=exitcode.SERIAL_ERROR)
        else:
            error(f"Unable to open serial device `{portname}`, {str(e)}", code=exitcode.SERIAL_ERROR)

    if not args.file:   # if no file to send, report status of serial device and exit
        sys.exit(exitcode.OK)

    stream = args.file
    printq(f"Sending `{stream.name}` ({os.stat(stream.name).st_size} bytes): ", end='')

    success = False

    def getc(size, timeout=1):
        ch = port.read(size)
        # echo 'x' to report failure if read timed out/failed
        if ch == b'':  # received no bytes
            if not success: printq('x', end='')
            return None
        return ch

    def putc(data, timeout=1):
        n = port.write(data)
        # echo '.' to report full packet successfully sent
        if n >= 128:
            printq('.', end='')

    try:
        xmodem = XMODEM(getc, putc)
        success = xmodem.send(stream, retry=5)
        if not success:
            error("Send failed (bootloader not listening?)",
                  "I waited a few seconds for an acknowledgement from the bootloader\n"
                  "and didn't hear anything. Do you need to reset your Pi?\n\n"
                  "Further help at https://cs107e.github.io/guides/bootloader/#troubleshooting",
                  code=exitcode.BOOTLOAD_FAIL)
    except serial.serialutil.SerialException as ex:
        error(str(ex), code=exitcode.SERIAL_ERROR)
    except KeyboardInterrupt:
        error("Canceled by user pressing Ctrl-C.",
              "You should probably restart the Pi, since you interrupted it mid-load.",
              code=exitcode.USER_CANCEL)

    printq(bcolors.OKGREEN + "\nSuccessfully sent!" + bcolors.ENDC)
    stream.close()

    if args.p:  # after sending, -p will loop and echo every char received
        try:
            while True:
                if sys.stdout.isatty() and select.select([sys.stdin,],[],[],0.0)[0]:  # user has typed something on stdin
                    sys.stdin.readline()  # consume input and discard
                    print(bcolors.FAILRED + "Huh? Did you intend to type that on your PS/2 keyboard?" + bcolors.ENDC)
                c = getc(1)
                if c == b'\x04':   # ascii control char for end of transmission sent by Pi on normal exit
                    printq(f"\n{args.exename}: received EOT from Pi. Detaching.")
                    sys.exit(exitcode.EOT)
                elif c == b'\x18':  # ascii control char for cancel sent by Pi as termination signal
                    printq(f"\n{args.exename}: received termination code from Pi, program forcibly exited. Detaching.")
                    sys.exit(exitcode.PI_TERMINATE)
                if c is None: continue
                print(c.decode('ascii', 'replace'), end='')
                sys.stdout.flush()
        except KeyboardInterrupt:
            printq(f"\n{args.exename}: received Ctrl-C from user. Detaching.")
            sys.exit(exitcode.USER_CANCEL)
        except serial.serialutil.SerialException as ex:
            error(str(ex), code=exitcode.SERIAL_ERROR)

    elif args.s:  # after sending, -s will exec `screen`, name the session so can find it later
        screen_cmd = f"screen -S rpi {portname} 115200"
        ans = input(f"{args.exename}: ({screen_cmd}) Start screen now? [y/n] ")
        if ans.lower() == "y":
            sys.exit(os.system(screen_cmd))
        else:
            printq(f"{args.exename}: screen canceled.")

    sys.exit(exitcode.OK)  # successful bootload, nothing further is known
