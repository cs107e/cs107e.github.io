---
title: Using the SD Card library to read and write files
---

*Written by Chris Gregg*

This tutorial describes how to read and write files from the internal SD card on the Raspberry Pi.

1. Download the [example](sd_example.zip) program, and unzip.
2. Perform a `make install` in the directory. You should see the following output:


    Mounted SD card.
    Creating file testfile.txt on the SD card.
    Wrote:
    Shall we play a game?
    Love to. How about Global Thermonuclear War?
    
    
    to testfile.txt, and wrote a total of 67 bytes.
    Read the following from testfile.txt:
    Shall we play a game?
    Love to. How about Global Thermonuclear War?
    
    
    Read a total of 67 bytes.
    Removed testfile.txt from the SD card.
    
    
    rpi-install.py: received EOT from Pi. Detaching.

3. Follow the examples in the example file to perform your own file operations. Check the `$(CS107E)/include/ff.h` file for the available functions, and see <a href="http://elm-chan.org/fsw/ff/00index_e.html">FatFs</a> for details about how to use the functions.

*Notes*:
1. A bare-metal Raspberry Pi doesn't keep time in the sense that it knows the current time of day, or day of the year. So, keep that in mind if you are writing to files -- unless you implement a time-keeping system, you won't get correct timestamps on your files. There is one function that the FatFs library relies on (as far as I can tell) to get the time:

    `int _gettimeofday( struct timeval *tv, void *tzvp )`

This function populates the `timeval` struct and `tzvp` struct with the current system time information. You can investigate writing this function if you need correct time stamps on your files. 
