#!/bin/bash
#
# quick & dirty script to get the device associated with CP2012
# vendor id 10C4, product id EA60
#
# Updated Jan 2026 to add fallback to pick up any IO device if specific id fail

HAS_POWERSHELL=$(which powershell.exe)
if [ ! -z $HAS_POWERSHELL ]; then   # has powershell -> on WSL
    # find com port id on Windows side using VID/PID and translate to unix tty device
    COM=$(powershell.exe '(Get-WmiObject -query "SELECT * FROM Win32_PnPEntity" | Where-Object {$_.DeviceID -like "*VID_10C4&PID_EA60*"}).Name' | grep -o "\(COM[0-9]\)")
    if [ -z $COM ]; then
        # fallback take any serial device
        COM=$(powershell.exe "Get-CimInstance -Class Win32_SerialPort | Select-Object Name" | grep -o "\(COM[0-9]\)")
    fi
    if [ ! -z $COM ]; then
        DEV="/dev/ttyS${COM:3}"
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # use pyserial to look up by VID/PID
    DEV=$(python3 -m xserial.tools.list_ports -q "VID:PID=10C4:EA60" 2>/dev/null)
    if [ $? -ne 0 ]; then
        # might be fixed by pip3 install pyserial but don't suggest that, instead
        # fallback take any serial device found by ioreg
        DEV=$(ioreg -r -c IOSerialBSDClient -l -w0 | awk -F'"' '/"IOCalloutDevice"/ {print $4}' | fgrep usbserial)
    fi
else
    echo "Do not understand this OS. Instead use manual list of files ls /dev/tty*"
    exit 1
fi
if [ ! -z "$DEV" ]; then
    printf "%s\n" $DEV
    exit 0
else
    echo "Could not find CP2102 USB-serial device."
    echo "I looked through the serial devices on this computer and did not"
    echo "find a device associated with a CP2102 USB-serial adapter. Is"
    echo "your USB-serial adapter plugged in?"
    exit 1
fi
