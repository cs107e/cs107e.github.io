---
title: 'Guide: Install developer tools on Windows WSL'
toc: true
---

<script>
$().ready(function() {
    var elems = document.getElementsByClassName('language-console');
    for (const elem of elems) elem.className += ' console-ubuntu';
});
</script>

These are the instructions for setting up a development environment using the Windows Subsystem for Linux (WSL). We peppered the installation instructions with <i class="fa fa-check-square-o fa-lg"></i> __Check__ steps that confirm your progress through the steps. Use each to validate a step before moving on.  If you hit a snag, stop and reach out (forum, office hours) and we can help you out!

You may need to restart your computer a few times throughout this process, so it’s a good idea to save all work before starting so you can restart when prompted.

## Requirements
> __Got WSL?__ By now you should already have installed WSL, if you don't, please first follow the [WSL installation guide](../wsl-setup) first and then come back here to install the developer tools.
{: .callout-warning}

Run the commands below in your WSL terminal to confirm that you are running an appropriate version of Ubuntu and WSL.

{% include checkstep.html content="confirm Ubuntu and WSL version" %}
```console
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 22.04.3 LTS
Release:        22.04
Codename:       jammy
$ powershell.exe "wsl --list --verbose"
  NAME              STATE       VERSION
 *Ubuntu-22.04      Running     1
```

{: .callout-warning}
## Install some prerequisites
```console
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo apt install make libmpc3 gdb
````
## Install riscv-unknown-elf toolchain

We use a cross-compiler toolchain to compile programs for the Mango Pi. Run the commands below in your WSL terminal to install the toolchain.

1. Download the prebuilt toolchain files:
    ```console
    $
    $ wget https://web.stanford.edu/~kenny1g/riscv-107e-wsl-prebuild.tar.gz
    ```

2. Install the packages.
NOTE: The first command is to check your current directory. If you are not in the `tmp-debs` directory, you will need to change to that directory before running the `dpkg` commands.

   ```console
   $ sudo tar -xvf riscv-107e-wsl-prebuild.tar.gz -C /opt
   # At this point, you can delete the tar file if you want
   $ rm riscv-107e-wsl-prebuild.tar.gz
   # Symlink the binaries
   $ sudo ln -s /opt/riscv-unknown-elf-107e/bin/riscv64-unknown-elf-* /usr/local/bin/
   ```

{% include checkstep.html content="confirm compiler" %}
```console
$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc () 13.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
## Install xfel

Next, we need to install xfel (<https://github.com/xboot/xfel>), which is the tool that we use to communicate with your Pi (i.e., send code and other nifty tricks).

Here are the steps:

1. Go to the releases page for xfel (<https://github.com/xboot/xfel/releases/>) and download **xfel-windows-v1.3.2**. The file is labeled `xfel-windows-v1.3.2.7z` and should be 5.58 MB and was uploaded on June 10th, 2023.

2. In your downloads folder (just navigating on your normal Windows file explorer), you should see that file, which is a compressed folder. Click on it and then click "extract all" on the top. You should now have an unzipped folder named `xfel-windows-v1.3.2`.

3. Navigate into that folder, clicking on `xfel-windows-v1.3.2`, then another folder inside with the same name `xfel-windows-v1.3.2`, and then to another folder labeled `Drivers`. Inside, there should be an application labeled `zadig-2.8.exe`. Double-click on that, and then when asked if you want to allow the application to make changes to your device, click "yes".

4. By now, you should see a pop-up that looks like the below image. **Make sure your Mango Pi is plugged in for this part.** Click "Install Driver". You should get a pop-up on top that says "Installing Driver..." and once that finishes running, make sure you get a message that the driver was installed successfully. If not, make sure you call over a staff member. If so, you're good to go on!

    ![zadig.exe popup](../images/zadig.png){: .w-90}

5. Close zadig.exe and go back to the `xfel-windows-v1.3.2` folder. Note its absolute path in Windows---for example, it might look something like
    ```
    C:\Users\[Your name]\Downloads\xfel-windows-v1.3.2\xfel-windows-v1.3.2\
    ```
6.  **In your WSL terminal**, navigate to the folder where we're going to store binaries (like the xfel program!):
    ```console
    $ cd $CS107E/bin
    $ ls
    blink-actled.bin  mango-run  pinout.py
    ```

7. We're going to copy a few files from your Downloads folder (on the Windows side of your computer) to the WSL file system. To do this, we need to use the path that you found in Step 5. We're going to have to reference that path, starting from the `C:\` folder, within WSL.

    For example, if my path in that step was `C:\Users\killer-rabbit\Downloads\xfel-windows-v1.3.2\xfel-windows-v1.3.2\`, then I would access that folder from WSL as `/mnt/c/Users/killer-rabbit/Downloads/xfel-windows-v1.3.2/xfel-windows-v1.3.2/`. (Note that the slashes go in different directions in Windows and Linux---that's just how they're referenced). You might have to play around to figure out the exact path on your computer, but once you have, use these two commands from inside the `$CS107E/bin` folder:

    ```console
    $ sudo cp /mnt/c/[downloads path]/xfel-windows-v1.3.2/xfel-windows-v1.3.2/xfel.exe xfel
    $ sudo cp /mnt/c/[downloads path]/xfel-windows-v1.3.2/xfel-windows-v1.3.2/libusb-1.0.dll .
    ```

    If this worked, the following files should appear in your respository:
    ```console
    $ cd $CS107E/bin
    $ ls
    blink-actled.bin  libusb-1.0.dll  mango-run  pinout.py  xfel
    ```

8. Time to check if that worked! Go back to your `mycode` folder, and try running xfel. {% include checkstep.html content="confirm xfel" %}

    ```console
    $ cd ~/cs107e_home/mycode
    $ xfel

    xfel(v1.3.2) - https://github.com/xboot/xfel
    usage:
        xfel version                                        - Show chip version
        xfel hexdump <address> <length>                     - Dumps memory region in hex
        xfel dump <address> <length>                        - Binary memory dump to stdout
        xfel read32 <address>                               - Read 32-bits value from device memory
        xfel write32 <address> <value>                      - Write 32-bits value to device memory
        xfel read <address> <length> <file>                 - Read memory to file
        xfel write <address> <file>                         - Write file to memory
        xfel exec <address>                                 - Call function address
        xfel reset                                          - Reset device using watchdog
        xfel sid                                            - Show sid information
        xfel jtag                                           - Enable jtag debug
        xfel ddr [type]                                     - Initial ddr controller with optional type
        xfel sign <public-key> <private-key> <file>         - Generate ecdsa256 signature file for sha256 of sid
        xfel spinor                                         - Detect spi nor flash
        xfel spinor erase <address> <length>                - Erase spi nor flash
        xfel spinor read <address> <length> <file>          - Read spi nor flash to file
        xfel spinor write <address> <file>                  - Write file to spi nor flash
        xfel spinand                                        - Detect spi nand flash
        xfel spinand erase <address> <length>               - Erase spi nand flash
        xfel spinand read <address> <length> <file>         - Read spi nand flash to file
        xfel spinand write <address> <file>                 - Write file to spi nand flash
        xfel spinand splwrite <split-size> <address> <file> - Write file to spi nand flash with split support
        xfel extra [...]                                    - The extra commands
    ```

## Installation complete

Yay! If you made it this far, you should feel so proud! Head back to the [main installation guide](../devtools) to do one last set of checks.
