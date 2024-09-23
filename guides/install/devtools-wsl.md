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

{% include checkstep.html content="confirm Ubuntu and WSL version 1" %}
```console
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 24.04 LTS
Release:        24.04
Codename:       noble
$ powershell.exe "wsl --list --verbose"
  NAME              STATE       VERSION
 *Ubuntu-24.04      Running     1
```
> __Be sure you have WSL version 1__
> WSL version 2 is not compatible with the tools we use in this course.
{: .callout-warning}

## Install developer tools
Run the commands below in your WSL terminal.

1. Install prerequisites and base toolchain.
```console
$ sudo apt install build-essential gcc-riscv64-unknown-elf
```

1. Download archive of our custom debugger build and extract files into place.
```console
$ wget https://github.com/cs107e/homebrew-cs107e/raw/master/riscv64-gdb-wsl.tar.xz
$ sudo tar -xvf riscv64-gdb-wsl.tar.xz -C /usr/local/bin
$ rm riscv64-gdb-wsl.tar.xz  # remove archive, not needed
```

{% include checkstep.html content="confirm compiler and debugger with simulator" %}
```console
$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc () 13.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
```

```console?prompt=(gdb),$
$ riscv64-unknown-elf-gdb
GNU gdb (GDB) 13.2
... blah blah blah ...
(gdb) target sim
Connected to the simulator.
(gdb) quit
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
    $ cp /mnt/c/[downloads path]/xfel-windows-v1.3.2/xfel-windows-v1.3.2/xfel.exe xfel
    $ cp /mnt/c/[downloads path]/xfel-windows-v1.3.2/xfel-windows-v1.3.2/libusb-1.0.dll .
    ```

    If this worked, the following files should appear in your respository:
    ```console
    $ cd $CS107E/bin
    $ ls
    blink-actled.bin  libusb-1.0.dll  mango-run  pinout.py  xfel
    ```

8. Time to check if that worked! Go back to your `mycode` folder, and try running xfel.

{% include checkstep.html content="confirm xfel" %}

```console
$ xfel
xfel(v1.3.2) - https://github.com/xboot/xfel
usage:
    xfel version                            - Show chip version
    xfel hexdump address length             - Dumps memory region in hex
    xfel dump address length                - Binary memory dump to stdout
    ...
```

## Installation complete

Yay! If you made it this far, you should feel so proud! Head back to the [main installation guide](../devtools) to do one last set of checks.
