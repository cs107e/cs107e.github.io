## Assignment 6: From Keyboard to Screen

![Matrix Trinity Hack](images/shell.png)

In this assignment, you'll get a PS2 keyboard working with your
Raspberry Pi. This will let you type in commands and control the
Pi without needing to plug another computer into it.

### Learning goals

In this assignment, you will

1. Learn how PS2 scan codes work 
1. Write a small driver that correctly translates PS2 scan codes into
    ASCII characters that can be displayed on the screen
1. Write a console program that allows you to type in and execute
    commands.

### Assignment tasks

1. __Pull the assignment 6 project.__

    To start the assignment, find the `courseware/assign/assign6/code`
    directory. You may need to pull from the `cs107e/courseware` repository
    to get the latest code. 

    This project includes one file that will help you significantly,
    `ps2.h`. This file defines a few useful constants and two arrays,
    `lookup_table` and `shift_lookup_table`, which are 128-element `char`
    arrays that map key codes on the range 0x00-0x7f to ASCII characters.
    The first is for when shift is not enagaged (e.g., "a" or "5"), while the
    second is for when shift is engaged (e.g., "A" or "%").

    The project also has `keyboard.h` and `console.h`, which you will
    implement in this assignment. You will need to make your own
    `keyboard.c` and `console.c` files and include them in the
    Makefile as you make progress in the assignment.

    Finally, there is `rpi_logo.h` and `rpi_logo.c`, which you'll use
    to make a Raspberry Pi logo show up on your screen when someone
    types the `logo` command at the console.

    You will probably want to copy your lab6 framebuffer and graphics
    code into the code directory.

1. __Process PS2 scan codes.__ Recall from lecture that a PS2 scan code
    is an 8-odd-1 packet consisting of 11 bits. The first bit, the start
    bit, is low. The next 8 bits are data bits, least significant bit first.
    The tenth bit is a parity bit -- odd parity means that there should be
    an odd number of 1s in the data and parity bits. The 11th and final
    bit is the stop bit, which is always high.

    ![PS2 Packet Format](images/ps2.png)

    Implement `keyboard_init()` and `keyboard_read()` in `keyboard.h`.
    You should build on your lab code to do so: initalize GPIO pins 23
    and 24 to be inputs with pull-up resistors. Recall that PS2 sends
    a new data bit on the data line on a falling clock edge, so when
    reading pin 23 goes from high to low.

    At this point, whenever you read a scan code, translate it to an
    ASCII character and display it on the screen. Do not worry about
    shift or other complexities. You can display the typed character
    as a character in a fixed position on the screen using a single-buffered
    graphics library. This way, a letter will appear immediately when
    you type it. All you need is a while loop that reads a character
    and draws it.

    ![PS2 Scan Codes](images/scancode.gif)

    Note that since you are simply turning scan codes to characters, typing
    some keys might lead to strange symbols appearing on the screen.
    That's fine: what matters is that typing basic characters shows up
    correctly.

1. __Handle key press vs. key release.__

    When you press a key, the PS2 keyboard sends a scan code. When you
    release the same key, it also sends a scan code. This scan code
    is the same as the first one, except it is one byte longer: it
    has a leading `0xF0`. So typing z will cause the keyboard to
    send scan code `0x1A`. Releasing z will cause the keyboard to
    send scan code `0xF0 0x1A`. If you hold down z long enough
    the keyboard will send multiple press scan codes (auto-repeat).

    Modify your keyboard driver so it will only return characters
    that are pressed, not those that are released. For example, if the
    scan codes received are `0x1A`, `0x1A`, `0xF0 0x1A` and `0x34`,
    the calls to `keyboard_read()` would have returned z, z, g,
    and not z, z, z, g.

    Also, modify your driver so it ignores (reads and does nothing with)
    a value of `0xE0`. This code is used for some more advanced keys.
    Ignoring it means that you might see some funny behavior (e.g., pressing
    volume up will cause a b to be read), but that's OK. If you don't
    do this then pressing keys which have an `0xE0`, such as the right
    control key, will cause multiple characters to be read.

1. __Implement shift and caps lock.__

    Modify your driver so it correctly handles caps lock and shift.
    If you press a key while the shift key is down, `keyboard_read`
    should return the result of the character in `shift_lookup_table`.
    If you press the caps lock key, then shift should be enabled
    until caps lock is pressed again. If you press shift while
    caps lock is enabled, characters should be lower case.

1. __Lines of text.__

    At this point, you should be able to handle most characters typed
    at the keyboard. Implement a simple console that lets you type
    in commands. This means storing lines of text which typing feeds
    into. When someone types delete or backspace (scan code `0x66`), the
    console should delete the last character on the current line, or
    do nothing if the line is empty. When someone types return/enter
    (scan code `0x5A`), the console should call `console_execute()`
    with the contents of that line and go to the next line.

    Implement this with double-buffering, such that you redraw the
    screen after each key press that changes the visible text.

    You do not have to handle line wrap: if a line reaches the end of
    the screen you should stop accepting new key strokes.

    You do not have to handle scrolling: if the console reaches the bottom
    of the screen it should stay at the bottom line and delete the previous
    one when return is pressed.

1. __Pie!__

    Fill in `console_execute()` so that if someone types the command
    `logo` it draws the Raspberry Pi logo made available through
    `rpi_logo.h`. The logo is 320x240 (so it doesn't cause your program
    to take forever to load), and you should draw it in the center of
    the screen. You should return to the regular console when the next
    character is typed. You will need to include `rpi_logo.o` in your
    Makefile; we left it out until now because it makes your program
    much bigger and therefore slower to upload to the Raspberry Pi.

1. __Utilities__
    Also add a command `list` that
    lists all of the available commands in the line(s) below the current
    one then brings the user to a new line. Finally, add a command `clear`
    that clears the screen and brings the console back to the top line.

### Handing in

Commit your code to your repository.

### Extension

1. __Better command line__. Extend your command line so it explicitly
    displays where the cursor is (e.g., by inverting colors or a white line
    at the bottom) and allows the user to move back and forth with
    the arrow keys. So, for example, someone can type "lgo", then
    hit left arrow 2 times and type "o" to spell "logo". Hitting
    return at any point in the line should execute the entire command.
    Also, support history: allow the user to type the up arrow to recall
    prior commands and easily re-execute them. Finally, support scrolling,
    such that if the console reaches the bottom of the screen then previous
    lines move up to make room.



  
