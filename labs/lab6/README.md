---
released: true
permalink: /labs/lab6/
title: 'Lab 6: Drawing into the Framebuffer'
attribution: Lab written by Philip Levis and Pat Hanrahan, updated by Julie Zelenski
toc: true
readings: |
  [Prelab](/labs/lab6/#prelab-preparation) to prep before lab.
---
{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab6:__
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Followup on issues from previous quarter postmortem (issue #403)

__To prep for lab6:__
- [ ] Confirm lab has working monitors with HDMI cables (aim for 1 for every 2 students)

{% endcomment %}


## Goals

For your next assignment, you will implement library of simple graphics routines and use them to implement a text console for your shell. The goal of this lab is to review concepts and code in preparation.

During this lab you will:

- Connect your Pi's HDMI port to a monitor for video out
- Write code to allocate framebuffer memory and make changes to the pixel data
- Review C syntax for pointers to multi-dimensional arrays
- Read and understand code for drawing font glyphs

## Prelab preparation
To prepare for lab, do the following: 

1. Be up to date on [recent lecture content](/schedule/#Week6): __Graphics, 2-D arrays__
1. Organize your supplies to bring to lab
    - Bring your laptop (with full battery charge) and entire parts kit.
    - You do not need your PS/2 keyboard for this lab, it can stay home.
    - You will need a HDMI monitor to complete these exercises. We have monitors available in lab for shared use.

## Lab exercises

### 0. Pull lab starter code

Change to your local `mycode` repo and pull in the lab starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror lab6-starter
```
### 1. Confirm Pi video out

Each of you should connect your Pi to a monitor once to confirm the video output.
Partner up with your neighbor(s) to share a monitor for the rest of the lab exercises.

Grab one of the HDMI monitors in lab, plug in power cord.
The monitor should have an attached HDMI cable. Connect that cable to the mini-HDMI port on your Mango Pi.

Change to the directory `lab6/grid` and build and run the grid program using
`make run`. The monitor should display a grid of white lines on a black background.

### 2. The `fb` module

The `fb` module is responsible for allocating the framebuffer memory
and coordinating with the `de` and `hdmi` modules that control
the hardware peripherals to display the framebuffer pixels on the screen.

#### Review code of modules `de` and `hdmi`
Our reference libmango provides the implementation of the `de` (Display Engine)
and `hdmi` modules.
Peruse the module interfaces [$CS107E/include/de.h](/header#de) and [$CS107E/include/hdmi.h](/header#hdmi). You
'll note that the modules export a tiny number of functions.
We wrote those modules to support exactly and only the very specific features needed for our system.

Do a brief skim over the the implementation in files
[$CS107E/src/de.c](/src#de) and [$CS107E/src/hdmi.c](/src#hdmi). You will not modify this code and
do not need to make deep dive into it. Your `fb` module will interact with
the `de` and `hdmi` modules through limited public functions documented in the header files.

#### Initializing the framebuffer

Change to the directory `lab6/fb`. The directory contains these files:

```console
$ ls
Makefile      fb.c         main.c
```
The file `fb.c` contains code to initialize the framebuffer. The `fb` module
declares a struct to hold the module-level state. This struct has four fields:


- `width`, `height`: width/height of framebuffer in pixels
- `depth`: number of bytes per pixel (always 4 in our system)
- `framebuffer`: base address of the framebuffer memory

Review the code in the `fb_init` function. Discuss with your tablemates and
try to answer the questions below.

   1. How are total bytes calculated when allocating the framebuffer?
   2. What will be the color of the pixels in the newly created framebuffer?
   3. How does the framebuffer coordinate with the `hdmi` module to set the screen size?
   4. How does the framebuffer communicate with the `de` module to draw the pixels on screen?
   5. What kind of changes will be needed for the `fb` module to support double-buffering (i.e. separate front/back buffers)?

If you do not feel confident about your answers or do not agree, please ask us to help resolve!
You're ready for this check-in question.[^1]


### 2. Draw pixels
In directory `lab6/grid`, open the `grid.c` file in your text editor and modify the program in the following ways:

1. Change the grid code to draw horizontal lines in red and vertical lines in yellow.
  Remember from lecture that the B (blue) in BGRA is the lowest byte.

2. Change the grid code to draw a checkerboard pattern
   (alternating filled red and yellow squares). Pro-tip: define a helper function
   that draws a filled rectangle.

> __Here be dragons__ One critical fact to keep in mind when working with the framebuffer memory is that C does no bounds-checking on array indexes.
Writing to an index
outside the bounds of the framebuffer will simply overwrite data at the inappropriate location.
Such as transgression can create a strange variety of symptoms, including corrupting the framebuffer image
and/or locking up the pipeline to the display hardware. Should you encounter seemingly bizarre results
when writing the framebuffer, review how your code accesses its memory. On a related note, redzone
protection can be lifesaver in these situations. The malloc module of the reference libmango has it enabled, if you did not do the extension yourself.
{: .callout-danger }

Check-in with us and show off your checkerboard. [^2]

### 3. Multi-dimensional arrays

Pointers are ubiquitous in systems programming in C and one of the most
difficult concepts to master. Just when you are getting the hang of pointers
and one-dimensional arrays, now we're adding multi-dimensioned arrays into the
mix. The goal of this lab exercise is to review pointers and multi-dimensional
arrays in preparation for your next assignment.

One convenient way to represent images is with a two-dimensional array.
Treating it as a 2-D array can be easier than explicitly calculating offsets
into a one-dimensional array.

To start, here is a quick self-test:

* What is the difference between the following two declarations?
  Think about what operations are valid on `a` versus `b`. Also
  think about what memory is allocated.

      char *a  = "Hello, world\n";
      char b[] = "Hello, world\n";

* What is the difference between the following two declarations?

      int *p[2];
      int (*c)[2];

  You may find the __[cdecl tool](http://cdecl.org/)__ helpful in demystifying a complex C declaration.

Inspect the code in `lab6/pointers/pointers.c`. Compile the program using
`make`, run it on your Pi, and interpret the results. Ask questions about
anything that doesn't make sense to you and check-in with us to confirm your
understanding.

### 4. Fonts

One of the features of the `gl` module is to draw characters on the screen.
In order to do this, you need a *font*. Each character in the font is a little
image of the glyph pixels to draw.

![Font](images/Apple2e.bmp){: .zoom .w-100}

This famous font was used by the original Apple IIe. We chose it to
provide that extra-special retro touch for your graphical console.

Review the file [$CS107E/src/font.c](/src#font). It declares a `font_t` struct for representing a font and
defines the variable `font_default`:

```
/* from font.c */
typedef struct  {
    unsigned char first_char, last_char;
    int glyph_width, glyph_height;
    uint8_t pixel_data[];
} font_t;

static const font_t font_default = {
    .first_char = 0x21, .last_char = 0x7F,
    .glyph_width = 14, .glyph_height = 16,
    .pixel_data = {
        0x03, 0x00, 0x33, 0x00, 0xcc, 0x00, 0xc0, 0x3c,
        0x00, 0x30, 0x00, 0x30, 0x00, 0xc0, 0x03, 0x00,
        ...
        0x3f, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
     }
 };
```

The pixel data for the character glyphs is stored as a bitmap.  In a bitmap,
each pixel is represented by a single bit. If the bit is 'on', the pixel is to
be drawn in the foreground color; if 'off', the pixel is set to the background
color. We use a bitmap rather than full RGBA because it takes much less (1/32
) memory. This makes the font data much smaller, and hence faster to
upload to your Pi.

Below is a pictorial representation of `font_default` using green to display
each 'on' pixel in the bitmap. (click the image to see larger version):

![Font](images/apple2e-line.bmp){: .zoom .w-100}

The font bitmap stores the glyphs in a single line left-to-right, in order of ASCII value. The font bitmap contains glyphs for 95 total characters, from ASCII 33 to 127. The leftmost glyph is
character `!` (ASCII 33, hex 0x21), the neighboring glyph is
character `"` (ASCII 34, hex 0x22) and so on ending with rightmost glyph, a little checkerboard, (ASCII 127, hex 0x7f) which corresponds to ASCII delete.

Each glyph is the same size: 14 pixels wide and 16 pixels tall.  This is termed
a *fixed-width* font.

The bitmap is stored using an array of `uint8_t`
values. For example, the first two bytes in the array are `0x03, 0x00`.  Group
the 8 bits from the first byte and 6 bits from the second into the 14-bit
sequence `0b00000011000000`.  These 14 bits correspond to the top row of the
first glyph, the exclamation point. The bits indicate the vertical line for 
the exclamation point is 2 pixels wide and positioned in the center. 

Each line of the bitmap is 1330 pixels long (95 characters * 14 pixels wide),
and requires 1330 bits. If each glyph is 16 pixels tall, how many total
bytes are in the `pixel_data` array?

Look carefully at the function `font_get_glyph()` in `font.c` which copies a
single glyph from the font bitmap into a buffer.

* Trace the operation of `font_get_glyph` for character `&` (ASCII 38)? At what locations in `pixel_data` does it look to find the appropriate
bits? 

Now change to the directory `lab6/banner` and review the code in `banner.c`.
This program calls `font_get_glyph` to store the glyph image into `buf`. The
code then wants to go on to access `buf` as a 2-D array through the variable
named  `img`, but `img` is missing its declaration and initialization. Read and
follow the instructions marked `TODO:` to fix this issue. Compile and run and
it will print a banner to uart using your Pi's
font - neat!

You're ready for this check-in question.[^3]

<a name="speed"></a>
### 5. Drawing performance
After you have a working gl module for your assignment, you can take things to the next level by turning your attention to optimization. Small targeted changes to a heavy-traveled tight loop can reap stunningly big gains. This exercise is a fun exploration in small tweaks to get your graphics code moving wickedly fast.

Change directory to `lab6/speed` and review the source in the `speed.c`
file. The program defines a `redraw` operation and uses the timer to count the ticks during the function's execution. The purpose of `redraw` is to draw every pixel in the screen in the same color (i.e. operation akin to `gl_clear` ).

The `redraw0` function works correctly, but is naive to a fault. Build and run the
code as given and it will print the number of ticks to execute it. Write down this tick count, it will be the baseline to beat.  It is possible to gain a __speedup of 100x__ or even a __1000x__ over the redraw0 baseline!

Follow a stepwise process so you can see and measure the effect of each
modification in isolation:

1. Duplicate the code from `redrawN` to make function `redrawN+1`, i.e. `redraw0` -> `redraw1`.  Make a small change to the code in `redraw1` that you think will result in performance gain. Edit `main` to add a time trial for `redraw1`.

2. Make a rough prediction about the expected effect on runtime.

3. Build and run the new version to see whether the observed change in tick count matches your intuition. Where the results surprise you, try to figure out why the effect is different than expected. Poke around in the generated assembly or ask us questions.

Repeat this process, each time advancing from the best version so far and
making another small change.

Below are some suggested avenues to explore:

+ `redraw0` is making a __lot__ of calls to `gl_draw_pixel`. Each is incurring the overhead of a function call and the (redundant) bounds checking within the call.  The function `redraw1` bypasses `gl` instead getting the draw buffer from `fb` and writing the color components directly to the framebuffer memory. How much of an effect does this have?
+ How much of an improvement is it to write each pixel as a single 4-byte value instead of writing 4 bytes, one for each of the BGRA components?
+ Does changing the order the pixels are accessed make a difference, i.e. instead of looping row by column, what if you loop column by row? What about looping over the pixels as a 1-d array instead of nested loop in 2-d?
+ Hoisting a repeated operation in the loop body to outside the loop is always good idea. See those calls to `gl_get_width` and `gl_get_height` on each loop iteration -- Yeet!
+ Without changing your code, how much help can you get by activating compiler optimizations?  At `-O0`, all optimizations are disabled, `-Og` is modest step-up while remaining debug-friendly, `O1`, `-O2`, `-O3` are higher levels that are progressively more transformative. After editing the `CFLAGS` in Makefile, do a `make clean` to discard previous build products and then recompile anew and re-run.  Pro tip: rather than enter as two separate commands, use the shell operator `&&` to chain the two `make clean && make run`. You can now retrieve the single combo command from your shell history rather than having to repeat two separate commands. (Optimization for your overworked fingers? :-) What difference in speed do you observe when changing the optimization levels?
+ When going hard for optimization, reviewing the assembly is the best way to really see what's going on. The Makefile has a target to show the disassembly for a named function, try it now: `make redraw0.list` . Edit the Makefile CFLAGS back to `-O0` temporarily. Use the command `make clean && make redrawN.list` to see the disassembly of your best redraw N function with no compiler optimizations. Now edit the Makefile to `-Og` or -`O2` and repeat that command to review the assembly at different levels Can you work out how the generated assembly is changing at different optimization levels?
+ Look carefully at assembly for your best redraw compiled at `-O2`, paying close attention to which instructions are inside the loop and which are outside.  The loop body can be tiny -- just 1-2 instructions to write each pixel, which is awesome, but makes it all the more annoying to see that the overhead of the loop control itself contributes another 1-3 instructions to each iteration (increment/compare/branch) -- boo! How might you restructure the loop to issue fewer of these
overhead instructions and focus more on doing the meaty work of writing pixels? Here's an idea: change the loop body to set more than 1 pixel, how about doing 2 pixels and edit the loop control to advance by 2? Now you are only paying for loop overhead once per 2 pixels, instead of every pixel. This optimization is called _loop unrolling_. If 2 is good, is 4 better? What about 8 or 16 or 1000? (Note that last group of pixels of partial-size will need special case handling if total number of pixels is not an even multiple of your unroll count).
+ `gcc` can apply this last optimization for you! Edit the loop back to the original version that does a single pixel each iteration. Immediately before the start of the loop, add this compiler directive `#pragma GCC unroll N`. Replace `N` with the loop unroll constant to use. Look at the generated assembly and re-run for different values of N to get timing results.  How does gcc automatic unroll compare with your hand unrolling?


Share about the improvement you were able to achieve.[^4] Which changes surprised you by how effective they are?  Which changes didn't seem to move the needle much at all? Are you able to make some sense of which changes are most effective and why?  When you are writing `gl_clear` for your assignment 6, try to apply what you learned here to help it run briskly.

## Check in with TA

The key goals for this lab are to leave with a good understanding of the layout of the framebuffer memory and feel ready for starting on Assignment 6.[^5]

You will need access to an HDMI monitor for assignments 6 and 7. This can be any computer monitor, tv, or projector that accepts HDMI input. We have monitors available in lab for shared use. If you have access to an HDMI display elsewhere, feel free to give it a try. We have tested libmango's HDMI module on a smattering of monitors and expect it should work on others as well, but are a little wary of lurking bug or corner case that has yet to surface. Let us know if you run into trouble so we can help diagnose.  If the display you have access to does not have an HDMI cable or requires an HDMI mini-to-standard adapter, we keep some cables and adapters  on hand to lend. Look for the box on the low bookshelf in the front corner of lab. Please remember to return anything you borrow.

Our HDMI monitors and their attached cables are to stay in lab for all to use.  __Thank you for not taking our monitors out of the lab room and not detaching or removing their cables!__

<div class=checkinsheet markdown="1">
# {{ page.title }}
<div class="underline-name"></div>
Circle lab attended:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  _Tuesday_  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  _Wednesday_
<BR>
<BR>
Fill out this check-in sheet as you go and use it to jot down any questions/issues that come up.  Please check in with us along the way, we are here to help![^6]
</div>


[^1]: Talk us through your plan for extending the `fb` module to support double-buffering.
[^2]: Demo the snazzy squares of your checkerboard.
[^3]: Show the declaration for `img` needed for the `banner.c` program.
[^4]: Tell us about your efforts to improve redraw performance. What speedup over baseline were you able to achieve? What changes gave the biggest gains?
[^5]: Are there any tasks you still need to complete? Do you need assistance finishing? How can we help?
[^6]: Do you have any feedback on this lab? Please share!
