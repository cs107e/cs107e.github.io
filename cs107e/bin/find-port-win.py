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
import platform, re, subprocess, sys

# Set the vendor and product ID of the serial unit.
# The CP2102 units used during years 2018 - 2022 all have
# vendor ID 0x10C4 and product ID 0xEA60.
SERIAL_VID = "10C4"
SERIAL_PID = "EA60"

def error(msg, code=1):
    sys.stderr.write(f"{msg}")
    sys.exit(code)

# We used to just have a preset list --
# /dev/tty.SLAB_USBtoUART for macOS + Silicon Labs driver,
# /dev/cu.usbserial for Prolific,
# /dev/ttyUSB0 for Linux, etc.
# Instead find port by matching device with vendor/product id of our CP2102 board
def find_serial_port():
    portname = None
    # Device IDs aren't available in WSL. In this case, we do the search through PowerShell and map the output to the Linux device name
    if platform.system() == "Linux" and "Microsoft" in platform.release():
        ps_cmd = ['powershell.exe', '(Get-WmiObject -query "SELECT * FROM Win32_PnPEntity" | ' +
                  'Where-Object {$_.DeviceID -like "*VID_%s&PID_%s*"}).Name' % (SERIAL_VID, SERIAL_PID)]
        output = subprocess.check_output(ps_cmd).strip()
        match = re.search(rb'\(COM(\d+)\)', output)
        if match:
            portname = '/dev/ttyS' + match.group(1).decode('utf-8')

    if portname is not None:
        return portname
    else:
        error("Could not find CP2102 serial device.\n"
              "I looked through the serial devices on this computer, and did not\n"
              "find a device associated with a CP2102 USB-to-serial adapter. Is\n"
              "your Pi plugged in?")

def find_serial_port_alt():
    # Device IDs aren't available in WSL. In this case, we do the search through PowerShell and map the output to the Linux device name
    portname = None
    if platform.system() == "Linux" and "Microsoft" in platform.release():
        ps_cmd = ['powershell.exe', 'Get-CimInstance -Class Win32_SerialPort | Select-Object Name']
        output = subprocess.check_output(ps_cmd).strip()
        match = re.search(rb'\(COM(\d+)\)', output)
        if match:
            portname = '/dev/ttyS' + match.group(1).decode('utf-8')

    if portname is not None:
        return portname
    else:
        error("Could not find CP2102 serial device.\n"
              "I looked through the serial devices on this computer, and did not\n"
              "find a device associated with a CP2102 USB-to-serial adapter. Is\n"
              "your Pi plugged in?")

if __name__ == "__main__":
    portname = find_serial_port_alt()
    print(portname)
    sys.exit(0)
