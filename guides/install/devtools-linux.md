---
title: 'Guide: Install developer tools on Linux'
toc: true
---

<script>
$().ready(function() {
    var elems = document.getElementsByClassName('language-console');
    for (const elem of elems) elem.className += ' console-ubuntu';
});
</script>

Below are basic instructions for installing the dev tools on ubuntu. These are written for __ubuntu 24__ and we have done only limited testing. The instructions may not be compatible with a different version or alternate distro.

Before starting, confirm your version.

{% include checkstep.html content="confirm Ubuntu" %}
```console
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 24.04 LTS
Release:        24.04
Codename:       noble
```

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
## Build and install xfel

You also need xfel (<https://github.com/xboot/xfel>), a tool used to communicate with your Pi (i.e., send code and other nifty tricks).

1. Install prerequisites.
    ```console
    $ sudo apt install libusb-1.0-0-dev pkg-config
    ```
1. Clone sources for xfel.
    ```console 
    $ git clone git@github.com:cs107e/xfel.git
    ```
1. Build and install.
    ```console
    $ cd xfel
    $ sudo make install
    ```
{% include checkstep.html content="confirm xfel" %}

Confirm xfel is properly installed by running the command below.

```console?prompt=$
$ which xfel
/usr/local/bin/xfel
$ xfel
xfel(v1.3.2) - https://github.com/xboot/xfel
usage:
    xfel version                                        - Show chip version
    xfel hexdump <address> <length>                     - Dumps memory region in hex
    xfel dump <address> <length>                        - Binary memory dump to stdout
    ... blah blah blah ...
```

## Installation complete

Yay! If you made it this far, you should feel so proud! Head back to the [main installation guide](../devtools) to do one last set of checks.
