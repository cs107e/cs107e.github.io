---
released: true
permalink: /assignments/assign6/
title: 'Assignment 6: Graphics Library and Console'
toc: true
---
{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign6:__
- [ ] Review writeup/starter code (instructor)
- [ ] Review consistency/completeness of grading info published to students relative to grading tests used, consider changes clarity/ease of grading (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #183)

__To prep for assign6:__
- [ ]

{% endcomment %}

*Written by Philip Levis, updated by Julie Zelenski*

{% include duedate.html n=6 %}

![Wargames Console](images/wargames.jpg)

A console provides a command-line text interface for entering commands and seeing output.
Today we have fancy shell programs that support scripts, process
control, and output redirection. But simpler consoles can be
powerful too. One very famous console is
[Joshua in WarGames](https://www.youtube.com/watch?v=ecPeSmF_ikc).

### Goals

In this assignment, you will add graphics capability to your Pi and use it to create a snazzy graphical display for your shell. This will unleash your Raspberry Pi from the shackles of its laptop minder and elevate it into a standalone personal computer running a console that allows the user to enter and execute commands. Neat!

In completing this assignment you will have:

- learned how a framebuffer is used as a bitmap to drive a video display
- implemented simple drawing routines
- unlocked achievement of wizard-level proficiency with C pointers and multi-dimensioned arrays

After this there is one final assignment where you'll polish up a few details and improve the performance and then your top-to-bottom system will be complete. This is all bare-metal code you wrote yourself -- what an exciting achievement and sense of satisfaction you have earned with all your hard work!

## Get starter files
Change to the `cs107e.github.io` repository in your `cs107e_home` and do a `git pull` to ensure your courseware files are up to date.

To get the assignment starter code, change to your local repository, fetch any changes from the remote and switch to the assignment basic branch:
```
$ cd ~/cs107e_home/assignments
$ git fetch origin
$ git checkout assign6-basic
```

Verify that your project has up-to-date versions of
your modules `gpio.c`, `timer.c`, `strings.c`, `printf.c`, `backtrace.c` and `malloc.c`, `keyboard.c` and `shell.c`.  Use `git merge` to gather any changes from other branches. For example, if you made a regrade submission on `assign5-basic`, have the `assign6-basic` branch checked out and use `git merge assign5-basic` to incorporate those updates. 

If aiming to achieve the full system bonus in assignment 7, be sure that you are building and testing on your code, not the reference implementation. Consult the `Makefile` for information on configuring the build to use your modules.

The starter project contains the modules `fb.c`, `gl.c`, and `console.c`, the application program `apps/console_shell.c` and the test program `tests/test_gl_console.c`. You will edit `fb.c`, `gl.c`, and `console.c` to implement the required functions. You can add tests to `tests/test_gl_console.c`. The program `apps/console_shell.c` is used unchanged as a sample application which tests your console program.

The `make install` target of the Makefile builds and runs the sample application `apps/console_shell.bin`. The `make test` target builds and runs the test program `tests/test_gl_console.bin`. With no argument, `make` will build both, but not run. 

## Basic section

### 1) Framebuffer

The base layer of graphics support is implemented in the `fb` module which manages the framebuffer and communicates with the GPU using our provided `mailbox` module. The functions exported from `fb` are:

+ `void fb_init(unsigned int width, unsigned int height, unsigned int depth_in_bytes, fb_mode_t mode)`
+ `void* fb_get_draw_buffer(void)`
+ `void fb_swap_buffer(void)`
+ simple getters for fb settings: `fb_get_width`, `fb_get_height`, ...

The header file [fb.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/fb.h) documents the operation of each function.  

The starter version of `fb_init()` contains the code from lab6 to configure the framebuffer in single-buffered mode. In this mode, there is only one buffer. All drawing takes place in that one buffer and is immediately displayed on screen. The framebuffer's virtual size is configured to be equal to the physical size.

You will extend `fb_init` to support configuring the framebuffer in double-buffered mode. In this mode, the virtual height is set to be twice the physical height; this makes space for two screen-size buffers in the one virtual framebuffer. The lower half corresponds to one buffer and the upper half is the other. We refer to the buffer currently displayed on-screen as the 'front' buffer and the other buffer is the 'back' or 'draw' buffer. In double-buffered mode, all drawing is done to the back buffer and when ready, swapping the back buffer to front gives a smooth on-screen transition. 

To implement `fb_swap_buffer` you change which half of the virtual framebuffer is frontmost (displayed) by changing the Y offset from 0 to the physical height (or vice versa). To make this change, set the `y_offset` in the fb struct and write to the mailbox to inform the GPU to update.

After each call to `mailbox_write`, follow up with a call to `mailbox_read` to acknowledge the GPU's response. If you forget this, the mailbox queue eventually fills up and the system will hang.

The `tests/test_gl_console.c` test program has one very rudimentary test for the `fb` module. Be sure to augment the test program with your own tests that further exercise the functionality of the module. Your graphics library will be layered on the framebuffer and you need to be sure this foundation is solid before moving on.

### 2) Graphics primitives
The graphics library layers on the framebuffer and provides higher-level drawing primitives to set and get the color of a pixel, draw filled rectangles, and display text. 

Read the header file
[gl.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/gl.h) for documentation of the basic drawing functions:

+ `void gl_init(unsigned int width, unsigned int height, gl_mode_t mode)`
+ `void gl_draw_pixel(int x, int y, color_t c)`
+ `color_t gl_read_pixel(int x, int y)`
+ `void gl_draw_rect(int x, int y, int w, int h, color_t c)`
+ `void gl_clear(color_t c)`
+ `color_t gl_color(unsigned char r, unsigned char g, unsigned char b)`
+ simple getters for gl settings: `gl_get_width`, `gl_get_height`, ...

Review the provided `gl_init` code to initialize the framebuffer for 32-bit depth. Each pixel stores a 4-byte BGRA color. 

Start by knocking out the simple getter functions that wrap the underlying `fb` functions to provide a consistent `gl` interface for the client. The graphics routines call into `fb`, but the client doesn't need to know this. The client calls `gl_init` and `gl_draw....`, without any direct use of `fb`.

Next, move on to implement the `gl_draw_pixel` and `gl_read_pixel` functions. These set and get the color of an individual pixel. It is convenient to treat the framebuffer pixel data as a 2-D array and access pixels by x,y coordinates.  Review the syntax for multi-dimensioned arrays in the framebuffer lecture and the exercises of [Lab 6](/labs/lab6). Take care to compute the location of a pixel's data in the framebuffer based on __pitch__, not __width__, because the GPU may have made each row a little wider than requested for reasons of alignment.

When accessing the pixel data, be ever mindful that C provides no bounds-checking on array indexes. If you write to an index outside the bounds of the framebuffer, you step on other memory in active use by the GPU, with various dire consequences to follow. It is imperative that your functions detect and reject an attempt to read or write a pixel location that is out-of-bounds.

Filling a rectangle (`gl_draw_rect`) can be implemented as a nested loop to draw each pixel and clearing the screen (`gl_clear`) fills a screen-sized rectangle. Be sure to draw only those pixels which lie within the framebuffer bounds. You can enforce clipping by calling `gl_draw_pixel` for each pixel and letting that function sort out whether the pixel is in bounds. This simple approach is easy to get correct.

An alternate approach for clipping would be for `gl_draw_rect` to first intersect the rectangle bounds with the screen bounds and proceed to color only the pixels in the intersection, obviating the need for a repeated per-pixel check for in bounds. This code would run much faster.

Speaking of performance, thus far we have ignored it as our programs have all run acceptably fast without special effort.  Now that we are writing graphics code, we can hit bottlenecks that cry out for attention. There can __lots__ of pixels to process (2 million in a 1920x1080 display), so a `gl_clear` that executed 50 instructions per pixel would keep the Pi occupied for a full 5 seconds!  Reducing the number of instructions executed per pixel and streamlining the inner loop body will make a significant improvement.

For this assignment, we continue to prioritize correct functionality over efficiency, so it's fine to go with a simple-and-slow approach for now. We expect that your console will be sluggish and it will miss keys that are typed while it is in the middle of drawing. You'll fix this up in the final assignment by employing a mechanism for sharing the CPU during a long-running operation.

Some testing is now in order! Fire up the `tests/test_gl_console.c` test program and observe your graphics routines in action. Draw something that makes you happy! [SMPTE color bars](https://en.wikipedia.org/wiki/SMPTE_color_bars), the mandelbrot set, an animating automaton, crazy psychedelic patterns, ...


### 3) Fonts and text-drawing
The final two functions to implement for the graphics library are:

+ `void gl_draw_char(int x, int y, int ch, color_t c)`
+ `void gl_draw_string(int x, int y, const char* str, color_t c)`

Lab 6 introduced you to the [font.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/font.h)  module that manages the font image data.  A font has one combined bitmap consisting of glyphs for all characters, from which it can extract individual character images.

`gl_draw_char` will use `font_get_char` to obtain the character image
and then draws each 'on' pixel.  

`gl_draw_string` is simply a loop that calls `gl_draw_char` for each character, left to right in a single line.

Just as you did previously, ensure that you clip all text drawing to the bounds of the framebuffer.

Edit the test program to draw yourself a congratulatory message and add a variety of tests that exercise text drawing.  You're now ready to tackle the console.

### 4) Console
The console module uses the text-drawing functions of the graphics library to drive a monitor as a graphical output device. The console has these public functions:

+ `void console_init(unsigned int nrows, unsigned int ncols)`
+ `void console_clear(void)`
+ `int console_printf(const char *format, ...)`

These functions are documented in the header file
[console.h](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/include/console.h)                                                          .

The console module is a layer on top of the graphics
library, which itself is a layer on top of the framebuffer. The client interfaces with `console` by calling `console_init` and then
`console_printf`, without any direct use of `gl` or `fb`.

The console implementation stores the console contents, i.e., rows of text currently displayed, most likely using a 2-D array. The console also tracks the position of the cursor (insertion point). `console_init` initializes the contents to empty, `console_printf` adds text at the cursor position, and `console_clear` resets the contents to empty.

The `console_printf` function operates similarly to `printf` and uses your `vsnprintf` to prepare the formatted output. Once prepared, the characters to output are processed by the console one-by-one. Each ordinary character is inserted at the cursor position and the cursor advances. There are four special characters that require unique processing:
- `\b` : backspace (move cursor backwards one position)
- `\r` : carriage return (move cursor to first column in current row)
- `\n` : newline (move cursor down to first column of next row)
- `\f` : form feed (clear contents and move cursor to home position in upper left)

As it makes changes to the text contents, the console needs to update the display to match. You can make an incremental update as part of processing each character or postpone into a single refresh after processing a string of characters.

The console should also handle the operations for:
- Horizontal wrapping: if there are too many characters to fit on the current row, automatically wrap the overflow to the next row. It is a nice touch for backspace to correctly on a wrapped row, but we won't test this specific case in grading.
- Vertical scrolling: filling the bottommost row and starting a new one scrolls the text upwards, that is, all rows are shifted up by one. The top row scrolls off and the bottommost row now contains the text just added.

Trying to implement all of the console functionality, including special cases, in one go is a tough way to proceed. Instead, break the work down into manageable steps. For example, start by implementing a console of a single row that only processes ordinary characters. Add processing for backspace and carriage return. Use the test program to find and fix any issues before proceeding. 

Once your single row console is working, think about how you want to represent multiple rows. What kind of data structure will you use? How will you process newline and form feed?  Wrapping and scrolling add some unique complications. Think through what needs to happen for each and sketch out a plan before tackling that code.

Some ways of structuring the data will make the tasks much easier than others. We encourage you to talk
with your fellow students to discuss design tradeoffs.
Don't feel bound to one design: if you start implementing your
approach and it starts seeming very difficult, with many hard edge cases,
you may want to consider a new design. Like much code in this class, 80% of
the effort is figuring out exactly what your code should do. Once you have that worked out, it can be direct matter to write it. Throwing away a messy first attempt and using what you learned from it to restart with a clean design is often the best path forward; far better than grinding along with a flawed approach. 

### 5) Shell + console = magic!

The provided `console_shell.c` calls your `shell_init` passing
`console_printf` in place of the uart printf. With no change other than supplying a different output function, you now have a graphical version of the shell you wrote in assignment 5! 

If the shell feels slow or drops keys as you're typing, don't
worry. We'll fix that problem in the next assignment. Why might
the graphical shell be slow to process keys?

The video below demonstrates of our reference console. The shell is running on the Pi, user is typing on the PS/2 keyboard, and the output is displaying on the attached HDMI monitor.

<iframe width="625" height="400" src="https://www.youtube-nocookie.com/embed/Emfmc0fAJXY?modestbranding=1&version=3&playlist=Emfmc0fAJXY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Testing and debugging
The framebuffer and graphics modules can be exercised by adding tests to the `tests/test_gl_console.c` program. The use of `assert` with `gl_read_pixel` can be used to confirm that expected color at a given pixel location.  You can also run the test program and observe what is displayed to the monitor and visually confirm correctness.

For `console`, most of the testing focuses on correct operation of `console_printf`. You can edit the test program to feed all manner of format strings to `console_printf` to observe its handling of special characters, wrapping, and scrolling. Once you have worked through any kinks, try `apps/console_shell.c` program to test `console_printf` in the context of the graphical shell.

Let us sound a significant note of caution about the necessity to be vigilant whenever you access the framebuffer. The neighboring memory to the framebuffer contains critical data and should you erroneously write to an out of bounds location, you can cause all manner of havoc for the GPU. These transgressions can be punished in mysterious ways (screen garbage, a surprise reshowing of the Pi test pattern, crash/lockup of the GPU, etc), leading to much sadness and frustration. Be conscious of the automatic scaling applied for pointer arithmetic/array access and always know the units a value is expressed in (bits? bytes? pixels?).

## Extension: Line and triangle drawing
Create an `assign6-extension` branch for this part of the work and make a separate pull request from your basic submission.

Extend the graphics library so that you can draw anti-aliased lines and
triangles. Implement the function prototypes as given in `gl.h`.

This extension is true extra credit
and requires you to learn about line drawing algorithms.
A good starting point is the
[Wikipedia entry on line drawing](https://en.wikipedia.org/wiki/Line_drawing_algorithm).

Your line drawing function should draw *anti-aliased* lines:

![Anti-aliased Lines](images/antialiased.png)

Implement triangle drawing by using your line-drawing routine to draw the anti-aliased outline of the triangle and then fill the interior with the user's specified color.

Please note that pasting-and-modifying code you find online is not in the spirit of doing an extension and misrepresenting the work of others as your own is a violation of the Honor Code. You may read conceptual information and skim pseudocode to develop an understanding of an algorithm, but after reading, you should be able to put aside these references and write the C code yourself based on your own authentic understanding.

## Submit
The deliverables for `assign6-basic` are:

  - Implementation of the `fb.c`, `gl.c` and `console.c` modules
  - Your tests in `tests/tests_gl_console.c`

Submit the finished version of your assignment by making a git “pull request”. Make separate pull requests for your basic and extension submissions.

The automated checks make sure that we can run your C
code and test and grade it properly, including swapping your tests for
ours.

CI verifies that:

- `apps/console_shell.c` is unchanged

- `make` and `make test` successfully build

- `make test` also successfully builds with the unchanged version of the test program in the starter

## Grading
To grade this assignment, we will:

- Verify that your submission builds correctly, with no warnings. Warnings and/or build errors result in automatic deductions. Clean build always!
- Run automated tests that exercise the functionality of your `fb.c`, `gl.c`and `console.c` modules. We will also interactively test your console program running with a PS/2 keyboard and HDMI monitor.
- Go over the tests you added to `tests/test_gl_console.c` and evaluate them for thoughtfulness and completeness in coverage.
- Review your code and provide feedback on your design and style choices.

