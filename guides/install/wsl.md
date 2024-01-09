---
title: 'Guide: Install Toolchain on Windows WSL'
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
By now you should have WSL up and running on your device, if you don't, please follow the [WSL installation guide](../../wsl-setup) to get started. 

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

## Install riscv-unknown-elf toolchain

We use a cross-compiler toolchain to compile programs for the Mango Pi. Run the commands below in your WSL terminal to install the toolchain.

1. Download our toolchain package files (run each command one after the other):
    ```console
    $ mkdir tmp-debs && cd tmp-debs
    $ wget https://github.com/cs107e/homebrew-cs107e/raw/master/DebPackages/libc6_2.37-13_amd64.deb
    $ wget https://github.com/cs107e/homebrew-cs107e/raw/master/DebPackages/binutils-riscv64-unknown-elf_2.41-4+6_amd64.deb
    $ https://github.com/cs107e/homebrew-cs107e/raw/master/DebPackages/libisl23_0.26-3+b2_amd64.deb
    $ wget https://github.com/cs107e/homebrew-cs107e/raw/master/DebPackages/gcc-riscv64-unknown-elf_12.2.0-14+11+b1_amd64.deb

    ```

2. Install the packages.
NOTE: The first command is to check your current directory. If you are not in the `tmp-debs` directory, you will need to change to that directory before running the `dpkg` commands.

   ```console
   $ pwd
   $ sudo dpkg -i --auto-deconfigure --force-depends libc6_2.37-13_amd64.deb 
   $ sudo dpkg -i --force-depends libisl23_0.26-3+b2_amd64.deb
   $ sudo dpkg -i --force-depends binutils-riscv64-unknown-elf_2.41-4+6_amd64.deb 
   $ sudo dpkg -i --force-depends gcc-riscv64-unknown-elf_12.2.0-14+11+b1_amd64.deb
   ```

{% include checkstep.html content="confirm compiler" %}
```console
$ apt list gcc-riscv64-unknown-elf
Listing... Done
gcc-riscv64-unknown-elf/now 12.2.0-14+11+b1 amd64 [installed, local]


$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc (12.2.0-14+11+b1) 12.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
## Install xfel
> **TODO** xfel install instructions here
{: .callout-warning}

## Installation complete
