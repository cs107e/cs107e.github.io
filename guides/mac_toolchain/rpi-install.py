#!/usr/bin/python
import sys
import serial
from xmodem import XMODEM

if len(sys.argv) == 3:
    portname = sys.argv[1]
    filename = sys.argv[2]
elif len(sys.argv) == 2:
    portname = "/dev/tty.SLAB_USBtoUART"
    filename = sys.argv[1]
else:
    print "usage: %s tty bin" % sys.argv[0]
    sys.exit(1)


port = serial.Serial(port=portname, baudrate=115200)
if not port:
    print portname, 'not found'
    sys.exit(1)

stream = file(filename, 'rb')
if not stream:
    print filename, 'not found'
    sys.exit(1)

# problem was the timeout=0
#   needed to wait for NAK
def getc(size, timeout=1):
    return port.read(size)

def putc(data, timeout=1):
    port.write(data)

xmodem = XMODEM(getc, putc)

status = xmodem.send(stream)
print 'sent', status

stream.close()
sys.exit(int(not status))

