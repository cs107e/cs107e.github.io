## Assignment 1: Implement a Larson Scanner 

[Glen A. Larson](http://en.wikipedia.org/wiki/Glen_A._Larson), 
the man responsible for producing 
the original 
[Battlestar Galactica](http://en.wikipedia.org/wiki/Battlestar_Galactica)
and 
[Knight Rider](http://en.wikipedia.org/wiki/Knight_Rider_%281982_TV_series%29)
television series,
often used simple lighting effects in his shows.
One famous example is a scanner 
that consists of a set of red LEDs that scan back and forth.
The front of the Knight Rider car had a scanner.

![Knight Rider Car](images/knightrider.jpg)

Check out this short 
[video tribute](https://www.youtube.com/watch?v=hDUoyeMLxqM)
to Glen Larson. 
How many times does the scanner pattern appear?

Check out the Larson Scanner Kit Demo from
[evilmadscientist.com](https://www.youtube.com/watch?v=yYawDGDsmjk).

The goals of this assignment are

- To understand ARM assembly language.

- To become fluent with the ARM cross-development tools.

- To know how to use breadboards and use LEDs.

- To program general-purpose input/output (GPIO) pins.

### Basic Part: Simple Scan Pattern

This assignment involves modifying the blink program showed in class.
`blink.s` is an ARM assembly language program that blinks a single LED.
To implement a scanner, you will need to control at least 4 LEDs.

1 Wire up your breadboard with 4 LEDs. 
If you feel ambitious, 
and want to have make more interesting patterns,
wire up 8 LEDs.
If you use 8 LEDs, you will probably have to remove the buttons.
Make sure all the LEDs on your breadboard work.

2 Connect the LEDs to GPIO 20-23 (or GPIO 20-27, if you use 8 LEDs).

3 Copy `blink.s` to `larson.s` and add code to scan back and forth.
You will first need to initialize all the GPIO pins.
Then you will need need to sequentially turn LEDs on and off.
Make sure you handle the left and right sides properly
(that is,
the left-most and right-most LED should be on for only one cycle).
You should write the program without any function calls
(because we haven't taught you how to create and call functions 
in assembly language yet),
and only the assembly language instructions 
used in the original `blink.s`.

4 Generate the scanner pattern. 
Enjoy watching it late at night in your room.
Impress your friends.

### Extension

The basic part of this assignment should not take too long. 
If you want to explore further, extend the program.
These extensions are more challenging, and
may involve using additional ARM assembly instructions.

<!---
1 Program up 4 more patterns. 
If you want ideas,
check out some of these 
[patterns](https://www.youtube.com/watch?v=uUlkumlkryo#t=17).
-->

If you watch the Larson scanner carefully,
several LEDs are turned on at once.
The center one is at full brightness,
and the ones on the edge are dimmer.
How would you dim an LED?
Implement a scanner with LEDs dimmed 
in a way that would please Glen Larson.



