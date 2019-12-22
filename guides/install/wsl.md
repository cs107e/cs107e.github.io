---
title: Using WSL in CS107E
toc: true
---
Written by Rohan Deshpande, CS 107E Winter 2019_

Welcome to my comprehensive guide for using WSL in CS 107E! The following guide documents all the challenges that I've faced while using WSL this quarter and includes solutions that I've come up with. Currently, the only feature that WSL doesn't support (which you may need during the course) is `screen`. Make sure you have a VM handy if you need to use screen -- otherwise you should be fine for the duration of this quarter!

## Setup & Assignment 0

Start by installing your favorite flavor of linux. I strongly suggest using Ubuntu 16.04 over 18.04 since `arm-none-eabi-gdb` isn't included and 18.04. If you don't know what `arm-none-eabi-gdb` is, don't worry about it! It's a tool that you'll use later in the quarter that you'll need, so just use Ubuntu 16.04.

Once you have successfully installed WSL with the chosen flavor of linux, follow the instructions from `Assignment 0`. You shouldn't have any trouble with any of the commands used in this assignment since you're just using `git` (which works well with WSL).

## Using Sublime with WSL (optional but recommended)

Throughout the quarter, you will write/edit code using the text editor of your choice. Rather than stick with vim/emacs (which are command line text editors), you may want to edit files using Sublime Text. You can use this [link](https://www.sublimetext.com/) to download and install Sublime Text 3.

It is often helpful to be able to open a file in Sublime Text directly through command line, rather than having to open Sublime Text and navigate to the location of the file. This is really helpful when you're coding with multiple files and need to open/close them in Sublime. To integrate Sublime with WSL, you'll first have to find the location of your Sublime installation (on your Windows computer). For example, mine is in the directory `C:/Program Files/Sublime Text 3/subl.exe` (yours should be in a similar location). To check that you found the right path to the Sublime Text executable, type in the *absolute* path into terminal and hit enter. I typed:

```
$ "/mnt/c/Program Files/Sublime Text 3/subl.exe"
```

Note two things: You need to include `/mnt/` at the start to access your Windows files through terminal when using WSL, and I wrapped the command in quotes since the command has spaces in it. When you hit enter, you should see Sublime Text open successfully.

Rather than type the long command from above each time we want to use Sublime, we are next going to add Sublime as an alias. To do so, follow these steps:

1. Type in `vim ~/.bashrc`. This will open your `.bashrc` file in vim so that you can edit it.
2. Scroll to the bottom of the file with arrow keys. Near the bottom, you may see a line that reads `export CS107E=...`. You will see this line if you completed Assignment 0 correctly. You do not need to modify this line.
3. Hit `i` on your keyboard. This puts vim into INSERT mode. Add the following line to the bottom of the file: `alias subl='"/mnt/c/Program Files/Sublime Text 3/subl.exe"'`. Take note that the path should point to the Sublime Text executable path that you found in the previous step and make to include the quotes. This line creates an alias called `subl` that points to the Sublime Text executable.
4. Save your edits by hitting the ESC button on your keyboard and then type `:wq`. This will save your edits and quit out of vim.
5. Almost done! Just type in `bash` in the terminal or exit and reopen WSL. You should now be able to use Sublime Text with WSL.

### Testing Sublime - WSL Integration

Navigate to a folder of your choosing and pick a file that you want to edit. To open this file with Sublime, type `subl ` followed by the file name. For example, I'm trying to open a file titled `gpio.c` so I typed in `subl gpio.c`. If you did everything correctly, you should see the file automatically open up in Sublime!

## Lab 1: Setting up your Raspberry Pi

The prelab mentions that you may need to install arm cross-development tools depending on the OS environment that you're using. Since you're using Ubuntu 16.04/18.04, you don't need to worry about this step since the cross-dev tools come pre-installed.

### Step 5: SD Card

In step 5, the instructions tell you to plug in an SD Card and then navigate to the folder (in terminal) which corresponds to the SD card. 

> *Warning!* You probably won't see the SD card show up in the linux command line when you plug it in.

You will first need to mount the SD card before being able to see it in terminal. On your Windows machine, check File Manager / This PC under the Devices & Drives section to check where the SD Card was mounted. For example, my SD card appears under `D:`. To mount the drive type:
```
$ sudo mount -t drvfs D: /mnt/d
```
> Note: If your SD Card is mounted under a path other than `D:` you should change the part that reads `D:` and `/mnt/d` accordingly.

After completing this step, you should be able to navigate to the mount location (in my case `/mnt/d`) in order to add files to this location.

> Having trouble with mounting the SD Card? You don't have to do this step in terminal! You could just drag and drop files into your SD card as you normally would in Windows...

### Step 7: Bootloader

Start by following the instructions in step 7 from Lab 1. When you get to the part where you type `rpi-install.py blink.bin` it is likely that the python script won't be able to connect to your USB port even though it can find it. Use the USB port name from the error message (mine was `/dev/ttyS6`) to enter the following commands:
```
$ sudo chmod 666 /dev/ttyS6
$ stty -F /dev/ttyS6 -a
```
> Note: make sure you replace `/dev/ttyS6` with appropriate port if yours is different.

If you run `rpi-install.py blink.bin` again, the bootloader process should now work!

> *Important!* If you restart your WSL instance, it is possible that `rpi-install.py` will have trouble finding your USB port. If this happens all you have to do is type `sudo chmod 666 /dev/ttyS6` again and you should be good to go...

## Lab 3: Debugging and Testing

Part 2 of Lab 3 explores Serial communication by connecting TX and RX on the USB-serial adapter which causes characters sent out to be echoed back. To do this, the lab uses `screen`. Unfortunately, as of March 2019, screen isn't very stable in WSL as it depends heavily on which release of WSL you are using (and I wasn't able to get it working when I did the lab). I recommend using a VM for this part or at the very least working with someone who has screen working. If you *really* want to try to get screen working with WSL, I have two suggestions (which I haven't tried on the most recent release of WSL):

```
$ sudo /etc/init.d/screen-cleanup start
# above may work according to https://bugs.launchpad.net/ubuntu/+source/screen/+bug/574773

$ sudo screen 			# or try this (didn't work for me, but worth a shot)
```

## Other Labs & Assignments

You shouldn't have any trouble using WSL on any other labs / assignments. If you made it to the end of this guide, thanks for sticking around and happy coding with WSL!