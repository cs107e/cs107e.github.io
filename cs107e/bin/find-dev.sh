#!/bin/bash
#
# quick & dirty script to get the device associated with CP2012
# vendor id 10C4, product id EA60

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
    DEV=$(python3 -m serial.tools.list_ports -q "VID:PID=10C4:EA60")
fi
if [ ! -z $DEV ]; then
    echo $DEV
    exit 0
else
    echo "Could not find CP2102 serial device."
    echo "I looked through the serial devices on this computer, and did not"
    echo "find a device associated with a CP2102 USB-to-serial adapter. Is"
    echo "your Pi plugged in?"
    exit 1
fi
