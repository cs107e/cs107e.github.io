---
title: 'Guide: Install Windows WSL'
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
Confirm that you are running an appropriate version of Windows 11. To find the version number, type `winver` into the start menu search bar and run the command. A panel appears that reports the version information as shown in this screenshot:

![Window Version 22H2](../images/win11ver.png)
{: .w-75}

When we updated our installation instructions for Winter Quarter 2024, we tested on __Version 22H2__ of Windows 11 and recommend that your version match ours.
> __Windows version earlier than 22H2__ If you are running a Windows version that is earlier 22H2; you should not proceed with the installation instructions. Stop here and update to a compatible OS version first. If you need help, reach out to the staff.
{: .callout-danger-invert}

## Enable WSL and install Ubuntu 22.04

Windows OS does not natively support the development tools used in this course. Enabling WSL (Windows Subsystem for Linux) allows you to run an Ubuntu instance on top of Windows OS. You will install the necessary development tools into the Ubuntu WSL environment and do your coursework there.

### Enable and Install Ubuntu on the WSL Windows feature

1. Open PowerShell or Windows Command Prompt in administrator mode by right-clicking and selecting "Run as administrator", enter the wsl --install command below, then restart your machine.

    ```powershell
    wsl --set-default-version 1
    wsl --install -d Ubuntu-22.04
    ```

    > __Be sure to choose version Ubuntu 22.04 LTS__
    > The other Ubuntu versions do not support the software requirements we need.
    {: .callout-warning}

1. Once it's done, the `ubuntu` application wil be launched. And the terminal window will say "Installing, this may take a few minutes...".
1. Once that command finishes, it prompts you to enter a new username and password for your Ubuntu account. __Be sure to remember the name and password__, you'll need them later for administrator privileges.
1. Apply the latest Ubuntu updates by running the command below in your WSL terminal window:

    ```console
    $ sudo apt update && sudo apt upgrade
    ```
Do not be alarmed about the long-winded unix-babble produced -- these processes are total chatterboxes.

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
> __Be sure you have WSL version 1__
> The WSL version 2 is not compatible with the work we'll be doing in the class.
{: .callout-warning}

{%- comment %}
NOTE: WSL2 now supports serial ports
TODO: see if we need https://github.com/dorssel/usbipd-win
The rightmost column of the last command reports the version of WSL. It is critical that you are using WSL version 1, not 2, as serial port support is not available in WSL2.
{%- endcomment %}

You now have an up-to-date version of Ubuntu running in WSL on top of your Windows OS.

<a name="files"></a>
### Accessing WSL files from Windows

An important detail to note about WSL is that it hosts its own file system. The files you access within the WSL terminal are separate from your regular Windows file system. You can access the WSL files in the Windows File Explorer by changing to a particular directory in the WSL terminal and run the command below:

```console
$ explorer.exe .
```

This command opens a window in File Explorer that shows the files in the current WSL directory (whose shortname name is dot). Windows applications can now access these files, such as to edit a WSL text file using your favorite Windows text editor. Nifty!

## Installation complete

Nice job! Interested in becoming even more productive with WSL? It's okay if you don't have time now, but the ✨Windows-savvy✨ folks on the teaching team highly recommend using [Windows Terminal](https://github.com/microsoft/terminal), which wraps around the brutal Ubuntu default terminal to make it both pretty & full of very very useful features like tabs, rich text, and split terminals. If you have questions and want to do this, feel free to reach out during office hours!
