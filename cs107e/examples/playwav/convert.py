#!/usr/bin/env python
import wave, struct
from sys import argv
import sys
filename = argv[1]
ifile = wave.open(filename)

sampwidth = ifile.getsampwidth()
fmts = (None, "=B", "=h", None, "=l")
fmt = fmts[sampwidth]
dcs  = (None, 128, 0, None, 0)
dc = dcs[sampwidth]
cnt = 0
linecnt = 0

print "// Converted by AKWF2Teensy from %s." %(filename)
print "// AKWF2Teensy - Beerware by Datanoise.net."
print "const int16_t %s[257] = {" %(filename.replace(".wav",""))
for i in range(ifile.getnframes()):
    iframe = ifile.readframes(1)

    iframe = struct.unpack(fmt, iframe)[0]
    iframe -= dc
    cnt += 1
    if(cnt % 10 == 9):
        sys.stdout.write( "%6d,\n" %int(iframe))
        sys.stdout.flush()
        linecnt = 0
    else:
        sys.stdout.write( "%6d," %int(iframe))
        sys.stdout.flush()

# Rewind file. Need first frame as last one for linear interpolation.
ifile.rewind()
iframe = ifile.readframes(1)

iframe = struct.unpack(fmt, iframe)[0]
iframe -= dc
cnt += 1
print "%6d" %(iframe)

print "};"

ifile.close()
