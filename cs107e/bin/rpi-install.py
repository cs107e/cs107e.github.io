#!/usr/bin/env python3

"""
-------- VERSION numbering ----------
        First rpi-install.py by Pat Hanrahan, 2015-16
0.8     Edited by Omar Rizwan 2017-04-23.
0.91    Updated by Julie Zelenski winter quarter 2017-18
1.0     WSL support added Nov 2018 by mchang
1.1     Updated winter 2020 (python3, wsl)
-----------------------------------

This bootloader client is used to upload binary image to execute on
Raspberry Pi.

Communicates over serial port using xmodem protocol.

Should work with:
- Python 3
- any version of the on-Pi bootloader
- macOS and Linux

Maybe Cygwin and Ubuntu on Windows as well.

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
VERSION = "1.1"

# Set the vendor and product ID of the serial unit.
# The CP2102 units used fall 2018, winter 2019, winter 2020 all have
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
        os.path.basename(sys.argv[0]),
        bcolors.FAILRED + shortmsg + bcolors.ENDC + "\n" + msg
    ))
    sys.exit(1)

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
        error("Could not find CP2102 serial device.", """
I looked through the serial devices on this computer, and did not
find a device associated with a CP2102 USB-to-serial adapter. Is
your Pi plugged in?
""")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="This script sends a binary file to the Raspberry Pi bootloader. Version %s" % VERSION)

    parser.add_argument('file', help="binary file to upload", type=argparse.FileType('rb'), nargs='?')
    parser.add_argument('-d', help="serial device (e.g. /dev/ttyUSB0), otherwise search for CP2102 device", type=str, dest="port", metavar="device")
    parser.add_argument('-v', help="verbose logging of serial activity", action="store_true")
    parser.add_argument('-q', help=argparse.SUPPRESS, action="store_true")

    timeout = parser.add_mutually_exclusive_group()
    # -t specifies read imeout since last receiving a byte
    timeout.add_argument('-t', help=argparse.SUPPRESS, action="store", type=int, default=-1)
    # -T specifies total amount of time to read before closing channel
    timeout.add_argument('-T', help=argparse.SUPPRESS, action="store", type=int, default=-1)

    after = parser.add_mutually_exclusive_group()
    after.add_argument('-p', help="open serial connection and print output received from Pi",
                       action="store_true")
    after.add_argument('-s', help="start `screen` to send/receive with Pi",
                       action="store_true")

    args = parser.parse_args()

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
            error("The serial device `%s` is busy." % portname, """
Do you have a `screen` or `rpi-install.py` currently active on that device?
""")
        else:
            error("Unable to open serial device `%s`.\n%s." % (portname, str(e)))

    if not args.file:   # if no file to send, report status of serial device and exit
        sys.exit(0)

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

Further help at https://cs107e.github.io/guides/bootloader/#troubleshooting
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
                #  grade scripts invoke -T, don't print this message during autograde
                if args.T == -1 and select.select([sys.stdin,],[],[],0.0)[0]:  # user has typed something on stdin
                    sys.stdin.readline()  # consume input and discard
                    print(bcolors.FAILRED + "Huh? Did you intend to type that on your PS/2 keyboard?" + bcolors.ENDC)
                c = getc(1)
                if c == b'\x04':   # End of transmission.
                    printq("\nrpi-install.py: received EOT from Pi. Detaching.")
                    break
                if c is None: continue
                last_comm = time.time()

                print(c.decode('ascii', 'replace'), end='')
                sys.stdout.flush()
        except KeyboardInterrupt:
            printq("\nrpi-install.py: received Ctrl-C from user. Detaching.")
            sys.exit(0)
        except Exception as ex:
            print(ex)
            pass

    elif args.s:  # after sending, -s will exec `screen`, name the session so can find it later
        screen_cmd = "screen -S rpi %s 115200" % (portname)
        printq("rpi-install.py: %s" % screen_cmd)
        sys.exit(os.system(screen_cmd))

    sys.exit(0)
