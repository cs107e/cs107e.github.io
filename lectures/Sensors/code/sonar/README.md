http://www.bytecreation.com/blog/2013/10/13/raspberry-pi-ultrasonic-sensor-hc-sr04
    if lucky, get a web page vs a data sheet.
	- if unlucky, mostly works but doesn't.
	- cross reference.

    check stupid stuff.
	- wasted 5 minutes b/c unaligned jumper.

	- swap in 3v for 5v
		?

	- swap the pins.
 		- 5v out  3v
		- not resisted?

	(1) Using IO trigger for at least 10us high level signal,

	(2) The Module automatically sends eight 40 kHz and detect
	    whether there is a pulse signal back.  

	(3) IF the signal back, through high level , time of high output 
	    IO duration is the time from sending ultrasonic to returning.
	    Test distance = (high level time×velocity of sound (340M/S) / 2,
