#!/usr/bin/env python

"""
-------- VERSION numbering ----------
        First rpi-install.py by Pat Hanrahan, 2015-16
0.8     Edited by Omar Rizwan 2017-04-23.
0.91    Updated by Julie Zelenski winter quarter 2017-18
1.0     WSL support added Nov 2018 by mchang
-----------------------------------

This bootloader client is used to upload binary image to execute on
Raspberry Pi.

Communicates over serial port using xmodem protocol.

Should work with:
- Python 2.7+ and Python 3
- any version of the on-Pi bootloader
- macOS and Linux

Maybe Cygwin and Ubuntu on Windows as well.

Dependencies:

    # pip install {pyserial,xmodem}

"""
from __future__ import print_function
import argparse, logging, os, platform, re, serial, subprocess, sys, time
from serial.tools import list_ports
from xmodem import XMODEM

# See VERSION numbering above
VERSION = "1.0"

# Set the vender and product ID of the serial unit
# The CP2102 units from winter 2014-15 and spring 2016-17 both have
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

def error(shortmsg, msg=""):
    sys.stderr.write("\n%s: %s\n" % (
        sys.argv[0],
        bcolors.FAILRED + shortmsg + bcolors.ENDC + "\n" + msg
    ))
    sys.exit(1)

# We used to just have a preset list --
# /dev/tty.SLAB_USBtoUART for macOS + Silicon Labs driver,
# /dev/cu.usbserial for Prolific,
# /dev/ttyUSB0 for Linux, etc.
# Hopefully the device ID-based finder is more reliable.
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
        match = re.search(r'\(COM(\d+)\)', output)
        if match:
            portname = '/dev/ttyS' + match.group(1)

    if portname is not None:
        printq('Found serial port:', bcolors.OKBLUE + portname + bcolors.ENDC)
        return portname
    else:
        error("Couldn't find serial port", """
I looked through the serial ports on your computer, and couldn't
find any port associated with a CP2102 USB-to-serial adapter. Is
your Pi plugged in?
""")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="This script sends a binary file to the Raspberry Pi bootloader. Version %s." % VERSION)

    parser.add_argument("port", help="serial port", nargs="?")
    parser.add_argument("file", help="binary file to upload",
                        type=argparse.FileType('rb'))
    parser.add_argument('-v', help="verbose logging of serial activity",
                        action="store_true")
    parser.add_argument('-q', help="do not print while uploading",
                        action="store_true")
    parser.add_argument('-t', help="timeout for -p",
                        action="store", type=int, default=-1)
    parser.add_argument('-T', help="Total timeout for -p (the total amount of time to listen, as opposed to -t which specifies a timeout since last receiving a byte)",
                        action="store", type=int, default=-1)

    after = parser.add_mutually_exclusive_group()
    after.add_argument('-p', help="print output from the Pi after uploading",
                       action="store_true")
    after.add_argument('-s', help="open `screen` on the serial port after uploading",
                       action="store_true")

    args = parser.parse_args()

    def printq(*pos_args, **kwargs):
        if not args.q:
            print(*pos_args, **kwargs)
            sys.stdout.flush()

    logging.getLogger().addHandler(logging.StreamHandler())
    if args.v: logging.getLogger().setLevel(logging.DEBUG)

    if args.port:
        portname = args.port
    else:
        portname = find_serial_port()

    try:
        # timeout set at creation of Serial will be used as default for both read/write
        port = serial.Serial(port=portname, baudrate=115200, timeout=2)

        # Opening the port seems to always pull DTR low, so go ahead 
        # and perform complete reset sequence no matter what. If DTR 
        # unconnected, behaves as no-op.
        if args.v: printq("Toggling DTR pin to reset Pi: low... ", end='')
        port.dtr = True  # Pull DTR pin low.
        time.sleep(0.2)  # Wait for Pi to reset.
        if args.v: printq("high. Waiting for Pi to boot... ", end='')
        port.dtr = False  # Pull DTR pin high.
        time.sleep(1)     # Wait for Pi to boot.
        if args.v: printq("Done.")

    except (OSError, serial.serialutil.SerialException):
        error("The serial port `%s` is not available" % portname, """
    Do you have a `screen` or `rpi-install.py` currently running that's
    hanging onto that port?
    """)

    stream = args.file
    printq("Sending `%s` (%d bytes): " % (stream.name, os.stat(stream.name).st_size), end='')

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
            error("Send failed (bootloader not listening?)", """
I waited a few seconds for an acknowledgement from the bootloader
and didn't hear anything. Do you need to reset your Pi?

Further help at http://cs107e.github.io/guides/bootloader/#troubleshooting
""")
    except serial.serialutil.SerialException as ex:
        error(str(ex))
    except KeyboardInterrupt:
        error("Canceled by user pressing Ctrl-C.", """
You should probably restart the Pi, since you interrupted it mid-load.
""")

    printq(bcolors.OKGREEN + "\nSuccessfully sent!" + bcolors.ENDC)
    stream.close()

    initial_comm = last_comm = time.time()
    if args.p:  # after sending, -p will loop and echo every char received
        try:
            while True:
                if args.t > 0 and time.time() - last_comm > args.t:
                    printq("\nrpi-install.py: waited %d seconds with no data received from Pi. Detaching." % args.t)
                    break
                if args.T != -1 and time.time() - initial_comm > args.T:
                    printq("\nrpi-install.py: ran for a total of %d seconds. Detaching." % args.T)
                    break

                c = getc(1)
                if c == b'\x04':   # End of transmission.
                    printq("\nrpi-install.py: received EOT from Pi. Detaching.")
                    break
                if c is None: continue
                last_comm = time.time()

                print(c.decode('ascii', 'replace'), end='')
                sys.stdout.flush()
        except Exception as ex:
            print(ex)
            pass

    elif args.s:  # after sending, -s will exec `screen`, name the session so can find it later
        screen_cmd = "screen -S rpi %s 115200" % (portname)
        printq("rpi-install.py: %s" % screen_cmd)
        sys.exit(os.system(screen_cmd))

    sys.exit(0)
