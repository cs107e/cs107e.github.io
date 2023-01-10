---
title: "Guide: Reset your Pi without Unplugging"
toc: true
---

*Written by Anna Zeng, updated by Julie Zelenski*

Tired of repeatedly unplugging and replugging your USB-serial adapter to reset your Pi? Read on to learn how to solder a new header that accesses the Pi's processor reset pin. Connect a hardware or software switch to the new header and then resetting your Pi is as simple as flipping the switch.  Handy!

> **New to soldering?** Start with this [excellent tutorial](https://sites.google.com/stanford.edu/soldering-internal/learning), written by PhD student Max Holliday from the Stanford EE maker space [Lab64](http://lab64.stanford.edu).
{: .callout-info}

## Solder RUN header
In the photo below, look for the red circle around two contacts labeled `RUN`. The RUN contact is connected to the reset pin on the processor. Pulling this pin low will reset the Pi.

![](../images/reset.pin.ingredients.jpg){: .zoom}

Grab a 2-pin header (available in our lab cabinet) and place onto the RUN contacts as shown below:

![](../images/reset.pin.placement.jpg){: .zoom}

Heat the soldering iron to approximately 600 degrees F (60 on the dial), and wait until the light blinks rapidly.

![](../images/reset.pin.solder.light.jpg){: .zoom .w-25}

While holding the soldering iron like a pen, touch the solder on one side of the pin and hold the soldering iron on the other side. The iron will heat up the pin and pad and the liquid solder will flow over the joint and forms a little volcano.

![](../images/reset.pin.soldering.jpg){: .zoom }
![](../images/reset.pin.action.jpg){: .zoom }

Once the joint is complete, remove the soldering iron and the solder. Repeat for the second pin. The contacts will look sort of like this:

![](../images/reset.pin.done.jpg){: .zoom}

✔️__Check__

If you temporarily connect the two header pins (such as with a wire or pair of metal tweezers), your Pi should restart. This is a good test to verify your solder connections.

## Hardware reset switch
Attach two jumpers to the RUN header and connect the pins through a button. The button will act as hardware reset switch. Simply press the button to reset the Pi. Neat!

![](../images/reset.pin.button.jpg){: .zoom}

## Software reset switch
To take it to the next level, make a software-controlled switch by connecting  the RUN header to your USB-serial adapter.

Find the pin labeled `DTR` on your USB-serial. Use a jumper to connect the DTR pin to the RUN pin on your Pi. (We used a white jumper in the photos below.)

![](../images/reset.dtr.location.jpg){: .zoom }
![](../images/reset.dtr.run.jpg){: .zoom }

With this connection in place, grounding DTR on the USB-serial will reset the Pi. Our `rpi-run.py` script grounds DTR as a first step as a convenience for you. If DTR is unconnected, this action has no effect, but now that you have DTR connected to the RUN pin, this will reset the Pi. This automatically restarts the bootloader so it is ready to receive the new program. No more USB plugging and unplugging for you. Welcome to bootloader nirvana!


