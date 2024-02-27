---
title: 'Lab 7: Need for speed'
---

As a fun bonus exercise, let's take a look at what you can do when you're done
writing correct code: optimization. With this, your code can start moving
wickedly fast -- seriously.

Change directory to `lab7/speed` and review the source in the `speed.c`
file.

The program has the `redraw` function from the interrupts exercise, along with some timer scaffolding to count the ticks during the function's execution. The purpose of `redraw` is to draw every pixel in the screen in the same color.

The given version works correctly, but is naive to a fault. Build and run the
code as given to get a baseline tick count. *It is possible to gain more than a 1000x speedup over the redraw0 function!*

Take a stepwise approach, so you can measure the effect of a given
modification in isolation:

1. **Duplicate the previous function and tweak it.** For example, make a copy of `redraw0` and rename it `redraw1`. Make a small change to the code in `redraw1` that you think will impact performance.

2. Make a rough prediction about the expected effect on runtime.

3. Build and run the new version to see whether the observed timing matches your intuition. Where the results surprise you, try to figure out why the effect is different than expected. Poke around in the generated assembly or ask us questions.

Repeat this process, each time advancing from the best version so far and
making another small change.

Below are some suggested avenues to explore:

+ Hoisting an unnecessarily repeated operation outside the loop is always good idea. Those calls to `gl_get_width` and `gl_get_height` on each loop iteration have gotta go! 
+ There are a __lot__ of calls to `gl_draw_pixel`. Each is incurring the overhead of a function call and the (redundant) bounds checking within the call.  If you bypass `gl`, get the draw buffer from `fb` and write each pixel directly to the framebuffer, it will have a significant effect.
+ Does changing the order the pixels are accessed make a difference, i.e. instead of looping row by column, what if you loop column by row?
+ What about looping over the pixels as a 1-d array instead of nested loop in 2-d?
+ Here's something that can be tried without changing the code: edit the Makefile to enable various levels of compiler optimization and
rebuild and re-run. What difference do you observe between `-O0`, `-Og`, `-O2` and `-Ofast`?
+ Think about where the function spends time. Recall that every instruction 
contribute a cost: are there ways to change the function so that it does the 
same work with fewer instructions? Take a look at the assembly in `speed.list` to see where the effort is going. It takes just a few instructions to write a pixel but each loop iteration adds the overhead cost to increment, compare, branch. How could you change the loop to issue fewer of these
overhead instructions and focus more on doing the meaty work of writing pixels?

How big of an improvement were you able to make overall? Where did you get the biggest bank for your buck?
