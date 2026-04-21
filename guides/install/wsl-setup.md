---
title: 'Guide: Set up Windows WSL'
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
Confirm that you are running an appropriate version of Windows 11. To find the version number, type `winver` into the start bar and run the command. A panel appears that reports the version information as shown in this screenshot:

![Window Version 23H2](../images/winver23H2.png)
{: .w-75}

When we updated our installation instructions for __Fall 2024__, we tested on __Version 23H2__ of Windows 11 and recommend that your version match ours.
> __Old Windows version? Stop here!__ If your version of Windows version is earlier than what we tested (23H2); do not proceed with the installation instructions. Update to a compatible OS version first. If you need help, reach out to the staff.
{: .callout-danger-invert}

### Enable WSL Windows feature

Windows OS does not natively support the development tools used in this course. Enabling WSL (Windows Subsystem for Linux) allows you to run an Ubuntu instance on top of Windows OS. You will install the necessary development tools into the Ubuntu WSL environment and do your coursework there.

1. Type `features` into the start bar and open the Control panel to `Turn Windows features on or off`.
1. In the panel, find the checkbox `Windows Subsystem for Linux`. Check the box, and click `OK`. Wait for it to complete the requested changes and then restart your computer when prompted.

### Install Ubuntu 24.04

1. Open PowerShell or Windows Command Prompt and enter the commands below.
    ```powershell
    wsl --set-default-version 1
    wsl --install -d Ubuntu-24.04
    ```

    > __Be sure to install version Ubuntu 24.04__
    > The other Ubuntu versions do not support the software requirements we need.
    {: .callout-warning}

1. The `ubuntu` application is now launched and opens a new WSL terminal window. The terminal prints the message "Installing, this may take a few minutes...", be patient while it works.
1. After it completes, it may prompt you to create a username and password and if so, follow its instructions.
1. Bring the Ubuntu packages up to date by running the commands below in your WSL terminal window:

    ```console
    $ sudo dpkg-divert --rename --add /usr/bin/systemd-sysusers
    $ sudo ln -sf /usr/bin/true /usr/bin/systemd-sysusers
    $ sudo apt update && sudo apt upgrade
    ```
{% comment %}
REMINDER TO SELF: systemd is janked under WSL version 1. Call to systemd-sysusers will fail which errors out on package install. systemd isn't even used in WSL 1, safe to disregard, the trick is to do in a way that doesn't generate a bunch of scary warnings and/or block other installs. We use dpkg-divert to redirect future updates to systemd-sysuers, then replace systemd-sysusers with a no-op.  This clever solution brought to you by Michael Chang.
{% endcomment %}
The update procedure is very chatty, expect a lot of long-winded unix-babble to go by. Be patient and wait to confirm that the process eventually completes with no errors.

{% include checkstep.html content="confirm Ubuntu and WSL version 1" %}
Use the commands below to confirm the correct versions of WSL and ubuntu.
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

{%- comment %}
2024: we hear that WSL2 should now support serial ports, but we couldn't get xfel/minicom on board with it
Continue with strategy of WSL1 + access to USB on the Windows side.
FUTURE TODO?  https://github.com/dorssel/usbipd-win
{%- endcomment %}

You now have an up-to-date version of Ubuntu running in WSL on top of your Windows OS.

<a name="files"></a>
### Accessing WSL files from Windows

It is important to recognize that WSL hosts its own file system. The files you access within the WSL terminal are separate from your regular Windows file system. You can access the WSL files in the Windows File Explorer by changing to a particular directory in the WSL terminal and run the command below:

```console
$ explorer.exe .
```

This command opens a window in File Explorer that shows the files in the current WSL directory (whose shortname name is dot). Windows applications can now access these files, such as to edit a WSL text file using your favorite Windows text editor. Nifty!

## WSL setup complete

Nice job! Interested in becoming even more productive with WSL? The Windows-savvy folks on the teaching team highly recommend using [Windows Terminal](https://github.com/microsoft/terminal), which wraps around the basic Ubuntu default terminal to make it both pretty & full of very useful features like tabs, rich text, and split terminals. If you have questions and want to do this, feel free to reach out during office hours!
