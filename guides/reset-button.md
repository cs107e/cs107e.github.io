---
title: Reset the Pi without unplugging
toc: true
---

*Written by Anna Zeng, updated by Jennifer Lin*

Tired of repeatedly unplugging and replugging your USB-serial adapter to reset your Pi? Read on to learn how to solder a new header that accesses the Pi's processor reset pin. Connect a hardware or software switch to the new header and then resetting your Pi is as simple as flipping the switch.  Handy!

{% include callout.html type="warning" %}
**New to soldering?** Start with this [excellent tutorial](https://sites.google.com/stanford.edu/soldering-internal/learning), written by PhD student Max Holliday from the Stanford EE maker space [Lab64](http://lab64.stanford.edu).
</div>

### Solder RUN header
In the photo below, look for the red circle around two contacts labeled `RUN`. The RUN contact is connected to the reset pin on the processor. Pulling this pin low will reset the Pi.

[<img title="Things you will need" src="../images/reset.pin.ingredients.jpg" width="400">](../images/reset.pin.ingredients.jpg)

Grab a 2-pin header (available in our lab cabinet) and place onto the RUN contacts as shown below:

[<img title="Pin placement" src="../images/reset.pin.placement.jpg" width="400">](../images/reset.pin.placement.jpg)

Heat the soldering iron to approximately 600 degrees F (60 on the dial), and wait until the light blinks rapidly.

[<img title="Soldering Light" src="../images/reset.pin.solder.light.jpg" width="400">](../images/reset.pin.solder.light.jpg)

While holding the soldering iron like a pen, touch the solder on one side of the pin and hold the soldering iron on the other side. The iron will heat up the pin and pad and the liquid solder will flow over the joint and forms a little volcano.

[<img title="Soldering Position" src="../images/reset.pin.soldering.jpg" width="50%" style="display:inline;">](../images/reset.pin.soldering.jpg)[<img title="Soldering Action" src="../images/reset.pin.action.jpg" width="50%" style="float:right;">](../images/reset.pin.action.jpg)

Once the joint is complete, remove the soldering iron and the solder. Repeat for the second pin. The contacts will look sort of like this:

[<img title="Finished Soldering" src="../images/reset.pin.done.jpg" width="400">](../images/reset.pin.done.jpg)


✔️__Check__

If you temporarily connect the two header pins (such as with a wire or pair of metal tweezers), your Pi should restart. This is a good test to verify your solder connections.

### Hardware reset switch
Attach two jumpers to the RUN header and connect the pins through a button. The button will act as hardware reset switch. Simply press the button to reset the Pi. Neat!

[<img title="Reset Button" src="../images/reset.pin.button.jpg" width="300">](../images/reset.pin.button.jpg)

### Software reset switch
To take it to the next level, make a software-controlled switch by connecting  the RUN header to your USB-serial adapter.

Find the pin labeled `DTR` on your USB-serial. Use a jumper to connect the DTR pin to the RUN pin on your Pi. (We used a white jumper in the photos below.)

[<img title="DTR pin location" src="../images/reset.dtr.location.jpg" width="50%" style="display:inline">](../images/reset.dtr.location.jpg)[<img title="DTR pin to RUN pin connection" src="../images/reset.dtr.run.jpg" width="50%" style="float:right;">](../images/reset.dtr.run.jpg)

With this connection in place, grounding DTR on the USB-serial will reset the Pi. Our `rpi-install.py` script grounds DTR as a first step as a convenience for you. If DTR is unconnected, this action has no effect, but now that you have DTR connected to the RUN pin, this will reset the Pi. This automatically restarts the bootloader so it is ready to receive the new program. No more USB plugging and unplugging for you. Welcome to bootloader nirvana!


