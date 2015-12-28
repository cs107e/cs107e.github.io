## Mac installs

For this class, you will need to install both:

1. the `arm-none-eabi-` tools (including a special C compiler which
compiles C to _Pi_ machine code, instead of your computer's x86 machine
code) and
2. the driver and script for the CP2102 console cable (which connects
your computer and Pi, so you can load programs onto the Pi).

If you are on a Mac, follow all the instructions on this page.

### arm-none-eabi installation for OS X

To ease the installation process for the arm-none-eabi tools on OS X,
we have built and packaged them up for you. Follow these steps to
install them:

1. Download the [arm-none-eabi.tar.bz2 file](https://github.com/cs107e/courseware/blob/master/guides/mac_toolchain/arm-none-eabi.tar.bz2?raw=true).

2. We will run the tools from a bin subdirectory in your home
folder, instead of a computer-wide bin, so make this directory
first (provided you don't have one already):

   ```
   $ mkdir ~/bin
   ```

3. Copy the downloaded file into your bin directory:

   ```
   $ cp ~/Downloads/arm-none-eabi.tar.bz2 ~/bin/
   ```

4. `cd` into the bin directory and decompress and unpack the .tar.bz2
file (you should see a lot of files being extracted):

   ```
   $ tar -xjvf arm-none-eabi.tar.bz2
   ```

5. Your tools should now be installed. To check, try the following
   command and make sure the output matches:

   ```
   $ ~/bin/arm-none-eabi/bin/arm-none-eabi-as -v
   GNU assembler version 2.24 (arm-none-eabi) using BFD version (GNU Binutils) 2.24
   ```

6. While the above command works, it is annoying to have to specify
the directory of the installed tools each time you use them. To avoid
this, we can add the directory to our $PATH variable so that the shell
will search this directory when we type a command.

   To do this, open or create a file called .bash_profile in your home
directory (~/.bash_profile) and add the following line:

   ```
   export PATH=$PATH:~/bin/arm-none-eabi/bin
   ```

7. If you close that shell window and open a new one, you should now be able to simply type

   ```
   $ arm-none-eabi-as -v  
   GNU assembler version 2.24 (arm-none-eabi) using BFD version (GNU Binutils) 2.24
   ```

### Console cable driver & script

To use the CP2102 console cable, you will need to install the drivers.

Instructions on how to do this are in the
[Console Guide](../console.md).

Also download the
[rpi-install.py script](https://raw.githubusercontent.com/cs107e/courseware/master/guides/mac_toolchain/rpi-install.py),
which you will be using to load your programs onto the Pi over the
serial cable.

If you try to run this script, you may run across errors that there is
"No module named serial" or "No module named xmodem". You will need to
install these Python modules.

If you do not have pip (Python package management) on your Mac, [download and 
install pip](https://pip.pypa.io/en/latest/installing.html). Once you have
pip, you can easily get the missing modules using

```
$ sudo pip install pyserial
$ sudo pip install xmodem
```

Once those are installed, you should be able to run the script. You
will probably want to add the path of wherever you put the script to
your PATH variable (see above) so that you can run it from any
directory. The [bootloader guide](../bootlader.md) will go over how to
load programs over the console cable.
