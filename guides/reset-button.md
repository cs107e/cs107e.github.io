---
title: Reset the Pi without unplugging
---

*Written by Anna Zeng, updated by Jennifer Lin*

To reboot the Pi without unplugging and replugging in the USB-serial adapter, we can make use of the reset pin on the Pi. In the photo below, look for the red circle around two contacts labeled "RUN". The "RUN" pin is connected to the reset pin on the processor. When this pin is temporarily pulled low, the Raspberry Pi will reset. 

[<img title="Things you will need" src="../images/reset.pin.ingredients.jpg" width="400">](../images/reset.pin.ingredients.jpg)


This guide walks you through soldering a 2-pin header onto the RUN contacts. You can connect jumpers from these pins to a hardware or software switch and then flip that switch to reset your Pi.  Handy!

#### Solder reset header
Grab a 2-pin header (available in our lab cabinet) and place onto the RUN contacts as shown below:

[<img title="Pin placement" src="../images/reset.pin.placement.jpg" width="400">](../images/reset.pin.placement.jpg)

Heat the soldering iron to approximately 600 degrees F (60 on the dial), and wait until the light blinks rapidly.

[<img title="Soldering Light" src="../images/reset.pin.solder.light.jpg" width="400">](../images/reset.pin.solder.light.jpg)

While holding the soldering iron like a pen, touch the solder on one side of the pin and hold the soldering iron on the other side. The iron will heat up the pin and pad and the liquid solder will flow over the joint and forms a little volcano.

[<img title="Soldering Position" src="../images/reset.pin.soldering.jpg" width="50%" style="display:inline;">](../images/reset.pin.soldering.jpg)[<img title="Soldering Action" src="../images/reset.pin.action.jpg" width="50%" style="float:right;">](../images/reset.pin.action.jpg)

Once the joint is complete, remove the soldering iron and the solder. Repeat for the second pin. The contacts will look sort of like this:

[<img title="Finished Soldering" src="../images/reset.pin.done.jpg" width="400">](../images/reset.pin.done.jpg)

If you temporarily connect the two header pins (such as by using a jumper or a pair of metal tweezers), your Pi should restart. This is a good test to verify your solder connections.

#### Hardware reset switch
Attach two jumpers to the reset header and connect the pins through a button. The button will act as hardware reset switch. Simply press the button to reset the Pi. Neat!

[<img title="Reset Button" src="../images/reset.pin.button.jpg" width="300">](../images/reset.pin.button.jpg)

#### Software reset switch
To take it to the next level, you can connect the RUN pin on the Pi to the DTR pin on your USB-serial adapter and then control the switch via software. When the USB-serial pulls the DTR pin to ground, it triggers the Pi to restart. Find the pin labeled "DTR" on your USB-serial adapter. Use a jumper to connect the DTR pin to the RUN pin on your Pi. (We used a white jumper in the photos below.)

[<img title="DTR pin location" src="../images/reset.dtr.location.jpg" width="50%" style="display:inline">](../images/reset.dtr.location.jpg)[<img title="DTR pin to RUN pin connection" src="../images/reset.dtr.run.jpg" width="50%" style="float:right;">](../images/reset.dtr.run.jpg)

With this connection in place, pulling DTR to ground will now reset the Pi. When sending a program to the bootloader using our `rpi-install.py` script, the first thing the script does is pull DTR to ground. This means it will automatically reset the Pi so it is ready to receive the new program. No more USB plugging and unplugging for you -- super-cool!

{% include callout.html type="warning" %}
Note for Windows VM: Installing reset header and connecting to a hardware switch works, but the VM does not support software reset via DTR. We are looking into it and will let you know if we come up with a workaround. (January 2019)


