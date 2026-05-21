---
released: true
permalink: /assignments/assign6/
title: 'Assignment 6: Graphics Library and Console'
attribution: Assignment written by Philip Levis, updated by Julie Zelenski
toc: true
---

{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign6:__
- [ ] Review writeup/starter code (instructor)
- [ ] Review consistency/completeness of grading info published to students relative to grading tests used, consider changes clarity/ease of grading (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #342)

__To prep for assign6:__
- [ ]

{% endcomment %}

{% include duedate.html n=6 %}

![Wargames Console](images/wargames.jpg)

A console provides a command-line text interface for entering commands and
seeing output. Today we have fancy shell programs that support scripts, process
control, and output redirection. But simpler consoles can be powerful too. One famous console from popular culture is [Joshua in WarGames](https://www.youtube.com/watch?v=ecPeSmF_ikc).

## Goals

In this assignment, you will add graphics capability to your system and use it to
create a snazzy graphical display for your shell. This will unleash your Mango Pi from the shackles of its laptop minder and elevate it into a
standalone personal computer running a console that allows the user to enter
and execute commands. Neat!

In completing this assignment you will:

- learn how a framebuffer is used as a bitmap to drive a video display
- implement simple drawing routines
- gain greater proficiency with C pointers and multi-dimensioned arrays

After finishing this assignment, the functionality of your system is complete.
All that remains for assignment 7 is to apply a final touch of usability polish
so you can type faster without dropping keystrokes.  This system is all bare-metal code you wrote
yourself -- what a great achievement and sense of satisfaction you have 
earned with all your hard work!

## Get starter files
Change to your local `mycode` repo and pull in the assignment starter code:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror assign6-starter
```

In the `assign6` directory, you will find these files:
- `fb.c`, `gl.c`, `console.c`:  library modules 
- `test_gl_console.c`:  test program with your unit tests
- `console_shell.c`:  application program that runs your shell, reading input from the PS/2 keyboard and displaying output to the console. You will use this program unchanged.
- `Makefile`: rules to build console_shell application (`make run`) and unit test program (`make test`)
- `README.md`: edit this text file to communicate with us about your submission
- `style_reflection.txt`: a text file that you will edit for your responses to the style reflection prompts
- `gl_mine.h`, `extension.c`: edit these files if doing the extension

The `make run` target builds and runs the sample application
`console_shell.bin`. Use this target as a final step to confirm the full
integration of your `fb`, `gl`, and `console` modules.  The `make test` target
builds and run the test program `test_gl_console.bin`. This test program
is where you will add unit tests. You will make heavy use of this
target throughout your development.

As with the previous assignment, there is no `make debug` option as the simulator does
not implement the necessary keyboard and monitor peripherals.

You can edit the `MY_MODULE_SOURCES` list in the `Makefile` to choose which
of your modules to build on. (See instructions for [use of
MY_MODULE_SOURCES](/assignments/assign3/#mymodules) in Assign 3) Configuring your
Makefile to build on all your past modules is a great way to get a preview of your progress toward
achieving the full system bonus.

## Core functionality

### 1) Framebuffer

The base layer of graphics support is implemented in the `fb` module.

Start by reviewing the header file `fb.h` (available in `$CS107E/include` or [browse fb.h](/header#fb)).

+ `void fb_init(int width, int height, fb_mode_t mode)`
+ `void* fb_get_draw_buffer(void)`
+ `void fb_swap_buffer(void)`
+ simple getters for fb settings: `fb_get_width`, `fb_get_height`, `fb_get_depth`

The `fb` module allocates and manages the
framebuffer memory and coordinates with the lower-level  `de` and `hdmi`
modules to display the framebuffer on an HDMI monitor.

The starter version of `fb_init` contains the code from lab6. It allocates the framebuffer memory for the requested width and
height and fixed 32-bit depth that supports a 4-byte BGRA color for each pixel.
This version of `fb_init` assumes single-buffered mode, in which there is only one buffer
into which all drawing takes place. This buffer is always active
and on-screen; any changes to the pixel data are immediately displayed.

You will extend `fb_init` to support configuring the framebuffer in
double-buffered mode. In this mode, two separate
framebuffers are allocated. The active buffer is the one currently displayed
on-screen; the other buffer that is off-screen is used as the
'draw' buffer. In double-buffered mode, all drawing is done to the off-screen draw buffer.
When drawing is complete, the client calls `fb_swap_buffer` to bring the draw buffer on-onscreen.  A single swap of which buffer is active exchanges the two buffers.

A second or subsequent call to `fb_init` should perform
a re-initialization. Any previous memory is deallocated and the framebuffer is
reset for the requested configuration.

Time to test! The `test_gl_console.c` test program provides a simple `test_fb`.
Our provided sample unit test configures the framebuffer and uses
assert tests to confirm the initial state. Read the
provided test code and work out what should be displayed if working correctly.
Then run the test program and confirm what is drawn to the display matches
the expected.

### 2) Graphics
The graphics library module layers on the framebuffer and provides higher-level
drawing primitives to set and get the color of a pixel, draw filled rectangles,
and display text. 

Start by reviewing the header file `gl.h` (available in `$CS107E/include` or
[browse gl.h](/header#gl)) to see the documentation of the basic drawing
functions:

+ `void gl_init(int width, int height, gl_mode_t mode)`
+ `void gl_draw_pixel(int x, int y, color_t c)`
+ `color_t gl_read_pixel(int x, int y)`
+ `void gl_draw_rect(int x, int y, int w, int h, color_t c)`
+ `void gl_clear(color_t c)`
+ `color_t gl_color(uint8_t r, uint8_t g, uint8_t b)`
+ simple getters for gl settings: `gl_get_width`, `gl_get_height`

Review the provided `gl_init` code that calls on the `fb` module to initialize the framebuffer.

Start by writing the simple getter functions that provide a consistent `gl` interface to the client. The
`gl` versions just turn around and call into `fb`, but the client doesn't need to know this. The
client calls `gl_init`, `gl_draw_`m `gl_etc...`, without any direct use of `fb`.

Next implement the drawing functions `gl_clear`, `gl_draw_pixel` and `gl_draw_rect`.
Mapping from a pixel's x,y coordinate to the appropriate location within the framebuffer
memory is made easier when you access the memory as a multi-dimensioned array.  For a refresher on
that syntax, review the [framebuffer lecture](/lectures/Framebuffer/code/clear/clear.c) and
the exercises of [Lab 6](/labs/lab6).

When accessing the pixel data, be mindful that C does __no
bounds-checking__ on array indexes. If you write to an index outside the array bounds, you step on other neighboring memory with
various sad consequences to follow. Take care to
access only valid memory! Your public functions must vet the locations requested by the client and clip drawing as necessary. For example, a call to `gl_draw_pixel` should ignore a request to draw at an x,y location that is outside the framebuffer bounds.
If asked to draw a rectangle that is partially/fully out of bounds, `gl_draw_rect` should clip the drawing to only those pixels that are in bounds.
One way for `gl_draw_rect` to enforce clipping is iterate over all locations and call the public function `gl_draw_pixel` for each and depend on `gl_draw_pxiel` to discard/ignore the invalid ones. This simple
approach is easy to get correct, but will be quite slow because of the
repeated checks and function call overhead. The faster alternative for `gl_draw_rect` is to first compute the clipped bounds (i.e. by intersecting requested rectangle with framebuffer bounds) and then directly set only those pixels that are now known to be in bounds.

We recommend that you start with a simple approach that makes it easy to get the functionality correct. Later on you can decide what changes to apply to get the performance you want. It is almost always an easier task to improve the performance of correct code than to fix the functionality of fast, broken code.

Time to test! The `test_gl_console.c` test program defines a
simple `test_gl` to get you started. The provided tests are quite basic, you will need to supplement with additional tests to confirm the full range of functionality. One possibility is to rig up
assert-based unit tests that make a `gl_draw_xxx` call followed by calls to
`gl_read_pixel` to confirm the pixel color at a location. This approach doesn't scale much beyond simple cases.  We do recommend asserts for edge cases or specific details that are not easily to visually confirm (i.e. a rectangle of width 10 spans exactly 10 pixels). Most of your testing will be by visually confirming the result that is drawn on the display -- that's the real deal! Make sure pixels that should have been clipped are not being drawn and
that you never write outside the bounds of the framebuffer memory.

> __Careful with memory!__ The primary source of debugging woes on this assignment are due to incorrect
> access to memory -- uninitialized pointer, index out of bounds, wrong level
> of indirection, incorrect typecast, misunderstanding about units or layout --
> there be dragons here!  Know the bounds on your arrays and always respect those bounds. Be
> conscious of the automatic scaling applied for pointer arithmetic/array access.
> Keep track of the units a value is expressed in (bits? bytes? pixels?).
> Be __especially vigilant when accessing the framebuffer memory__. Should you erroneously write
> outside the framebuffer bounds, the transgression can cause all manner of strange
> artifacts on the display, up to and including a crash/lockup that forces you to reset your Pi. Such
> symptoms are a sign you have bugs in how you access the framebuffer memory.
{: .callout-danger}

### 3) Text-drawing
The final two functions to implement for the graphics library are:

+ `void gl_draw_char(int x, int y, char ch, color_t c)`
+ `void gl_draw_string(int x, int y, const char *str, color_t c)`

An exercise in [lab 6](/labs/lab6) introduced you to the `font`
module that manages the font image data.  A font is represented as a bitmap
containing all glyphs. To get the glyph for a requested character,
the call `font_get_glyph` extracts the associated glpyh from the bitmap.

`gl_draw_char` uses the font glyph as a stencil to draw each 'on' pixel and `gl_draw_string` simply calls `gl_draw_char` for each character in the string.

Just as you did previously, ensure that you clip all text drawing to the bounds
of the framebuffer.

Edit the test program to draw a congratulatory message.  To test character drawing, visual inspection is probably
your best option. Be sure to have test
cases that confirm that character drawing is correctly clipped.

You're now ready to tackle the console!

### 4) Console
The console module manages the console text contents (i.e. rows of text) and uses the text-drawing functions of the graphics library to draw the text contents  on the display.

Review the header file `console.h` (available in `$CS107E/include` or [browse
console.h](/header#console)). The console has these public functions:

+ `void console_init(int nrows, int ncols, color_t foreground, color_t background)`
+ `void console_clear(void)`
+ `int console_printf(const char *format, ...)`

`console_init` initializes the text contents to empty, `console_printf` adds text at the cursor position, and `console_clear` resets the text contents to empty.

The `console_printf` function should call use your `vsnprintf` to prepare the
formatted output.  It then processes the characters in the formatted output
one-by-one to update the text contents.
An ordinary character is inserted at the cursor position and the
cursor advances. There are three special characters that require specific
processing:
- `\b` : backspace (move cursor backwards one position)
- `\n` : newline (move cursor down to first column of next row)
- `\f` : form feed (clear contents and move cursor to home position in upper left)

When processing characters, the console must also handle:
- Horizontal wrapping: if the character to be inserted does not fit on the current row,
  automatically wrap the overflow to the next row. It is a nice touch for backspace
  to work correctly on a wrapped row, but we won't test this specific case in grading.
- Vertical scrolling: filling the bottommost row and starting a new one moves the
  text contents upwards, that is, all rows are shifted up by one. The top row scrolls
  off and the bottommost row now contains the text just added.

Console drawing operates in double-buffer mode. After `console_printf` has prepared the formatted string and processed the characters to update the text contents, the last step is to redraw. In the back buffer, clear the screen and draw the updated text contents. A final single call to `gl_swap_buffer` brings the new contents on-screen in a single smooth transition.

We provide a very basic console test in `test_gl_console.c`.
You will need to add test cases for backspace, form feed, line wrap and scrolling. There is a lot
to test! The ultimate goal is for console to work as the output device for the shell, but we recommend postponing that final integration test to
the end. Trying to debug your console within context of the full shell program makes your job much more difficult, instead write targeted test cases using `console_printf` in `test_gl_console.c`.

#### Designing your console
The external interface of the console must match the specification as given in `console.h`
but the internal design of module implementation is left to you as a creative and open-ended task.

Past students have been successful applying the software design pattern "Model-View-Controller"
[MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller), and we agree that it works well here. MVC
separates the data (the model) from the interactions that change the data (the controller) and the ability
to display the data (the view). The __model__ for the console is the text contents. The __controller__ is the already-completed `shell` module. The `shell` receives characters typed by the user and adds them to the text contents by calling `console_printf`. `console_printf` updates the model in response and then redraws the __view__ , i.e. the graphical display, to show the updated text contents. Redrawing the view as a separate task after updating the model nicely decouples the two operations.  The anti-pattern of attempting to make `console_printf` update the text contents while simultaneously drawing to the screen makes for a messy implementation.

The console model manages the text contents and tracks the cursor position.
You might choose to store the text contents as a 1-D array of `char *` or
instead as a 2-D array of `char` or something else entirely.  Be sure to consider the implications
in terms of the code require to manage it. Some ways of structuring the data
can simplify the implementation tasks or have less opportunity for error.
Make choices that make your life easier!

When ready to implement, take it one small step at a time.
The code for the console is structurally complex and has some nuanced interactions to manage
(adding text, managing cursor, wrap, scrolling, backspace). Your best bet is to
decompose into small parts that
you can incrementally write, test, then build on. For example, you can start by
handling only a single line, then backspace, then multiple rows, then scrolling.
For unit tests, we recommend you sequence together calls to `console_printf` and confirm the visual results on screen. Start with simple outputs and work your way up to correct
handling of special characters, wrapping, and scrolling.

The console does a lot of drawing, calling gl operations that may be a bit pokey yet. Rather than try to get ahead of that now, better to design for simplicity and correctness, get all console features working, albeit somewhat sluggishly, and then go back to make more responsive.  The alternative of jumping straight to a complex design in pursuit of efficiency can lead to a long fight with difficult bugs at unclear benefit.


### 5) Shell + console = magic!

The final step is an easy but satisfying conclusion: use your console as
the output display for your shell.  The `make run` target builds the
application program `console_shell.c`. This program calls your `shell_init`
passing `console_printf` in place of the uart `printf`.  Now the shell you wrote in assignment 5
springs to life in graphical form! You don't need to write any new code for
this, just run the `console_shell` program and enjoy seeing your modules all
work together in harmony.

If the console shell drops keys as you're typing, don't worry.
We'll fix that problem in the next assignment. Why might the console shell miss keystrokes?

The video below demonstrates of our reference console. The shell is running on
the Pi, the user is typing on the PS/2 keyboard, and the output is displaying on
the HDMI monitor.

<video controls="controls" width="625"
       name="Assignment 6 demo" src="images/console_demo.mp4"></video>

 > __Dropped keys__ For assignment 6, it is given that  __your console will miss keys that are typed__ while it is in the middle of drawing. You'll fix this in assignment 7 by employing interrupts to share the CPU during a long-running operation and queue up key events so none are missed.
 {: .callout-warning}


## Extension
The core `gl` features are exactly and only what is needed for the console; so many stones left unturned! If you are eager to learn more about graphics or thinking ahead to possible projects with fancier outputs, adding to your graphics library now would be a wonderful extension.

- turbo-charge drawing
    - up to now, we haven't worried about performance tuning as our programs have run acceptably fast without special effort, but graphics routines that process literally millions of pixels can't be so carefree
    - take measurements before optimizing, identify which operations are slow,  make changes, iterate, report on progress (use [speed exercise from lab6](/labs/lab6#speed) as template)
    - tighten inner loops (hoist out redundant operations, unroll, hand assembly?)
        - cutting even a few instructions per pixel saves tens of millions total across a 1000x1000 framebuffer
    - a naive approach to clipping that sends all drawing through `gl_draw_pixel` has a lot of overhead can be trimmed!
    - _cacheing_ is a technique used widely in computer science. The basic gist is to save result to re-use rather than repeatedly recompute/refetch. For example, `gl_draw_char` calls `font_get_glyph` each time to extract glyph. If gl extract a glyph once on first use and saved it, using cached version on subsequent draw avoids repeating the work.
    - the reference console is reasonably responsive without much heroics. The approach we used was to choose a strategy that is simple to get correct (full redraw once at end of each call to `console_printf`, no incremental redraw/reuse of what was previously drawn) and worked at streamlining key drawing operations. This investment made all drawing faster, not only console, and console code was dirt simple. Win-win!
- level up the features of your graphics library
    - draw lines/wireframes/curves
        - anti-aliasing will make for much nicer smooth edges!
        - classic algorithms to look into: Bresenham, Xiaolin Wu, Gupta-Sproull
    - alpha-blending, transparency
        - blend draw color with existing background
    - filled shapes such as triangles, polygons, ovals
    - fill patterns such as gradients, hatching, textures
    - draw images, image filters (grayscale, invert, blur, sharpen)
        - handy to know that Gimp <https://www.gimp.org/> can export image file as C array
    - additional/replacement fonts
        - collection of open-source fonts to peruse <https://fonts.google.com/> or design your own!
        - GIMP export to C array also useful here
- speed up float operations
    - many geometric algorithms make heavy use of floating point, which is very slow in our default build environment that uses software emulation for fp. Changing to use hardware floating point is a huge boost.
    - research how to enable and build hard-float, will require edits to Makefile, use of inline assembly to enable
    - use timer to measure fp-intensive code on hard-float versus soft-float and report back about the gains
- console
    - handle ANSI escapes to display text colors <https://en.wikipedia.org/wiki/ANSI_escape_code>
    - improve console responsiveness
        - full redraw on each typed key is a likely bottleneck
        - first focus on improve base drawing performance (see above)
        - if further speed needed, consider console-specific rework such as incremental redraw or clever scroll
- use gl to draw something that makes you happy
    - a favorite flag, [SMPTE color
    bars](https://en.wikipedia.org/wiki/SMPTE_color_bars), the [Mandelbrot
    set](https://en.wikipedia.org/wiki/Mandelbrot_set), [Sierpinski's
    carpet](https://en.wikipedia.org/wiki/Sierpi%C5%84ski_carpet), ...
    - graph equations of form y = f(x) on cartesian plane using function pointers

- or ... ?? have a different idea in mind, come talk to us!

> __Just for fun extras?__
Want to add fun extras using code from elsewhere? Go for it! Perhaps there is a fancy feature you want to have in your library but are not up for the deep dive to write the code yourself. You can adopt code found online or generated by an LLM, just be sure to clearly cite where it came from and understand it will not be considered in grading. If you are submitting for the extension, we review only the code that you wrote for yourself. If your extension additionally includes some adopted code, it should be clearly identified to avoid confusion/misrepresentation.
{: .callout-info}


Edit `extension.c` for your extension program (may be either application or unit tests for extended features) and use `make extension` to compile and run. Use the same git workflow as core, i.e. edit on dev branch, regularly commit and push. Edit the `README.md` file with brief summary of what you did/learned for the extension. Apply tag `assign6-extension` to submit when done.

The grading review of extensions is done in-person, you'll give us a demo and share about your learnings. See times for extension demo/grading on OH schedule. We look forward to seeing your creation!

The same guidelines apply to the extension as to the core: __it is fine to use online resources and generative AI tools to answer your questions and help you learn new things, but not cool to use it to write or fix your code.__


## Style reflection

Here are the instructions for the [style reflection](style_reflection) to be submitted with this assignment.

## Submitting
The deliverables for `assign6-submit` are:

- implementations of the `fb.c` `gl.c` and `console.c` library modules
- unit tests for all modules in `tests_gl_console.c`
- `README.md` (possibly empty)
- your responses to the prompt questions in `style_reflection.txt`
- all `assign6-submit` files commit/tag/push by due date or before end of grace period

Additional for `assign6-extension`:

- extension program `extension.c`
- `README.md` with information on your extension
- use tag `assign6-extension`, commit/tag/push files by end of grace period

## Grading
To grade this assignment, we will:

- Verify that your submission builds correctly, with no warnings. Clean build always!
- Run our automated tests on your `fb` `gl`and `console` modules.
- Go over the tests you added to `test_gl_console.c` and evaluate them for
  thoughtfulness and completeness in coverage.
- Review your completed style reflection.

Our highest priority tests will focus on the core features for this assignment:

- Essential functionality of your library modules
  - `fb`
    - correct configuration of framebuffer
    - memory management
    - double buffering
  - `gl`
    - drawing pixels, rects, characters
    - correct clipping of all drawing
  - `console`
      - accept ASCII characters
      - handling of special chars (\n \b \f)
        - __we will not test__: insert tab nor backspace through tab
      - horizontal line wrap
        - __we will not test__: backspace through wrapped line
      - vertical scrolling
      - drawing performance should be reasonably responsive
        - not required to be scorchingly fast, but please not painfully slow!
        - ok that keys are dropped if typed during redraw (to be fixed in assign7, stay tuned!)

The additional tests of lower priority will examine less critical features, edge cases, and robustness. Make sure you thoroughly tested for a variety of scenarios!
