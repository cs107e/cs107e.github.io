---
title: Installing Linux VM on Windows
---

*Prepared by Philip Levis*

This installation involves downloading two large files, VMware
Workstation and a 3.2GB virtual machine, so can take a long
time. *Complete these steps before attending lab.*

If you have a Windows laptop, the easiest way to get a good development
environment for CS107E is to run Linux within a virtual machine. You'll
use Windows normally, but will have a window in which a full Linux system
is running.

We've prepared a VM with Linux and all of the needed tools installed on it.
To get it running, you'll need to do 4 things:

1. Install VMware workstation. Go to
   <https://sustainable.stanford.edu/campus-action/energy/sustainability-information-technology/vmware> 
   and scroll down to
   "SOFTWARE FOR ACADEMIC/RESEARCH PURPOSES" and follow the
   Software Licensing Store link. Choose VMware and
   click on VMware Workstation 14. This is a personal, not a lab,
   download. Use your own SUNet ID. When you check out, the
   checkout page will give you a download button as well as
   a license key to use. Copy the license number into a text file
   and save it.

2. Install VMware workstation and enter the license key.

3. Download the virtual machine. You can find it at
   <http://www.stanford.edu/~plevis/107E%20Ubuntu%20VM.zip>
   Note that it is 3.2GB in size: I recommend that you plug into
   a wired connection to download it. Don't put the VM on an external
   hard drive, since USB is much slower than SATA. Since you're running
   another whole computer which needs to load all of its data out
   of the VM file, this is like trying to run a full computer off of
   a USB drive: slow!

4. Unzip the virtual machine and run it. Open the .zip file
   and copy the directory into "My Documents/Virtual Machines".


5. Open VMware workstation and open the CS107E Ubuntu VM.
   Click on the green start icon. You may get a warning
   message or two: for SATA, tell it to always check, for
   USB, click OK (when I shut down the VM it had a Raspberry
   Pi plugged in). Your login is "Raspberry Pi Hacker" and
   the password is 'cs107e'. You can click on the terminal
   icon on the left to open a terminal. There is a subdirectory
   cs107e that has your tools and a little bit of source code
   in it.


