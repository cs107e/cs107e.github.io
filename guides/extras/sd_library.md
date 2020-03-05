---
title: Accessing files on SD card (libpisd.a)
---

The `$CS107E` directory contains a library `libpisd.a` that provides functionality for accessing the SD card from the Raspberry Pi. If you link with this library, you can call its functions to read and write files on the internal SD card. The library is based on FatFs, a filesystem specifically designed for use in embedded systems.

To get started with using `libpisi.a`:

- The header file `$CS107E/include/ff.h` declares the available functions with very brief explanation. Read the more complete documentation for the <a href="http://elm-chan.org/fsw/ff/00index_e.html">FatFs Application Interface</a>.
- There is an example project `sd_fatfs` in the directory `$CS107E/examples`. This test application demonstrates reading and writing files on the SD card. You can build and run the example application to try it out and review the code to see how it operates.
- To use the library, your Makefile should link with `libpisd` and the source file should `#include "ff.h"`. The example project demonstrates a correct build configuration that you can use as a model.


_A few cautionary notes:_
- This code was cobbled together from open-source code. Testing shows the basic operations are functional and it has been successfully used by student projects in previous quarters. But depending on how hard you push it, you might encounter operations that are missing/broken/buggy. Keep on your toes!
- The FatFs library relies on the `time` and `localtime` functions to set the modification time when writing a file. In our unhosted world, these functions are unimplemented, so a file written on the Raspberry Pi will have a bogus date for its modification time. You can investigate writing your own timekeeping functions to supply proper date/time information if you need correct timestamps. 

### Credits
-  Filesystem layer came from [FatFs by ChaN](http://elm-chan.org/fsw/ff/00index_e.html)
- Low-level sd controller came from [rpi-boot by John Cronin](https://github.com/jncronin/rpi-boot)


