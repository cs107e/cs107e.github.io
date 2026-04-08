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

These are the instructions for setting up a development environment using the Windows Subsystem for Linux (WSL). We peppered the installation instructions with <i class="icon-check-square-o"></i> __Check__ steps that confirm your progress through the steps. Use each to validate a step before moving on.  If you hit a snag, stop and reach out (forum, office hours) and we can help you out!

You may need to restart your computer a few times throughout this process, so it’s a good idea to save all work before starting so you can restart when prompted.

## Requirements
> __Got WSL?__ By now you should already have installed WSL, if you don't, please first follow the [WSL installation guide](../wsl-setup) first and then come back here to install the developer tools.
{: .callout-warning}

__Note: all terminal commands below should be entered in the WSL terminal__!

Before starting, confirm that you are running an appropriate version of Ubuntu and WSL.

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

## Install riscv-unknown-elf toolchain and prerequisites

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

1. Change to the proper directory, download the xfel archive, and unpack it. Replace `[YOUR-WINDOWS-USERNAME]` with the name of your __Windows__ login account (this is NOT the same as your ubuntu username!)
    ```console 
    $ cd /mnt/c/Users/[YOUR-WINDOWS-USERNAME]   # Windows username NOT ubuntu username
    $ wget https://github.com/cs107e/homebrew-cs107e/raw/master/xfelWin32.tar.gz
    $ sudo tar -xvf xfelWin32.tar.gz      # sudo to permit update file modtime
    $ rm xfelWin32.tar.gz                 # done with archive, remove
    ```
2. Run the "zadig-2.8.exe" to install the USB device driver. When application launches, it will pop up an alert that asks you to confirm it can make changes to your drivers, be sure to click "yes".
    ```console
    $ powershell.exe "./xfel/zadig-2.8.exe"  # install driver for usb
    ```

3. In the Zadig window that looks like screenshot below:
    - choose `Create New Device` from the `Device` menu
    - enter `Mango Pi` as the description text (may need to click the `Edit` checkbox first)
    - enter `1F3A` `EFE8` for the USB ID
    - choose `libusbK` from the popup next to the green arrow
    - click the large button that says `Install Driver` or 'Reinstall Driver'.
    The progress meter comes up and reports "Installing Driver..." as it works. Wait to comfirm the installation successfully completes and close zadig.

    ![zadig.exe popup](../images/zadig.png){: .w-90}

4. Now we will create a symbolic link for the xfel executable. This allows us to keep the executable in the Windows file system while still being able to call it from WSL (this arrangement is needed to avoid a painfully long antivirus scan by Windows Defender on every invocation of xfel).
    ```console
    $ sudo ln -sf /mnt/c/Users/[YOUR-WINDOWS-USERNAME]/xfel/xfel /usr/local/bin
    ```

    If this worked, xfel will be findable on your wsl path:
    ```console
    $ which xfel          # find xfel command and report its path
    /usr/local/bin/xfel
    ```

{% include checkstep.html content="confirm xfel" %}

Confirm xfel is installed by running the command below.

```console?prompt=$
$ xfel
xfel(v1.3.2) - https://github.com/xboot/xfel
usage:
    xfel version                                        - Show chip version
    xfel hexdump <address> <length>                     - Dumps memory region in hex
    xfel dump <address> <length>                        - Binary memory dump to stdout
    ... blah blah blah ...
```

{% include checkstep.html content="confirm xfel communicates with Mango Pi" %}
A more robust test is to confirm that xfel can communicate with your Mango Pi.  If you have your Mango Pi in hand, connect it to a USB port on your laptop and use `xfel version` to read the chip id.

```console?prompt=$
$ xfel version
AWUSBFEX ID=0x00185900(D1/F133) dflag=0x44 dlength=0x08 scratchpad=0x00045000
```

If `xfel version` instead responds with `ERROR: xfel cannot connect to usb device: -12`, your installation is not quite right. Double-check that you have WSL __version 1__ (NOT version 2) and the zadig driver is properly installed. Retrace the steps above to correct the problem and use `xfel version` to confirm.


## Installation complete

Yay! You have a cross-compile tool chain and a working xfel. Head back to the [main installation guide](../devtools) to do one last set of checks.
