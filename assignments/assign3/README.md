---
layout: page
title: 'Assignment 3: Implementing a string formatting library'
permalink: /assignments/assign3/
---

**Due: Monday, February 1, 2016 at 11:59PM**

Often when programming embedded computers,
space is at a premium.
Many of the C standard libraries are surprisingly large,
and it is useful to have simpler versions of standard functions available.

`printf` is particularly useful,
since printing is the most basic form of debugging.

The goals of this assignment are

- Understand C strings and characters.

- Understand how to convert basic C types (`int`, `int*`, ...) to a string.

- Use your gpio and timer code from the previous assignment.

- Use the uart library shown in class to communicate with your laptop.

This can be a difficult assignment, so start early and practice good development 
and debugging techniques like you learned in lab. Students who have struggled in 
the past wrote too much code before testing it. Try to implement this assignment in 
little increments, making the smallest change each time so if it doesn't work, 
you know exactly where to find the mistake.

Remember to decompose your code into simple, clearly-defined functions. This will 
make your code easier for you to debug and easier for your graders to understand. 
There are no length requirements, but as a guideline, the cleanest versions from 
previous years have included 10-15 functions and 200-300 lines of code.

### Basic Part: Basic formatting functions

1.   There is no starter code for this assignment.
     If you pull your private assignment repository and checkout your 
     assign3 branch, the only source files will be uart.h/c from lecture.
     Your first step will be to setup this assign3 branch with 
     a Makefile (we recommend modifying one from lab or assign2)
     and your gpio and timer functions you used in the clock assignment. 
     You should put the `gpio_set_function` and `gpio_write` 
     functions from the clock assignment in a file gpio.c, and the timer and 
     delay functions in a file timer.c. Then create header files for these two 
     files (gpio.h and timer.h) so that the functions may be imported into new 
     files. 

     Some of the gpio functions are required by uart.c, which 
     will be used in part 4 of this assignment. You 
     will need to set up your Makefile to compile all these files into object 
     files and link them together.

     Lastly create a new file `printf.c` where you will write your code for this 
     assignment.

2.   Implement the following basic formatting functions: 

     ```
     char* inttobin( char *buf, int n, unsigned val, int width); 
     char* inttodec( char *buf, int n, int val, int width); 
     char* inttohex( char *buf, int n, unsigned val, int width); 
     ```
     In these functions, `buf` is a character array to store 
     the string representing the number. 
     The size of the buffer is equal to n.
     Do not write past the end of the buffer!
  
     `val` parameter is the integer value to be converted to a string. When converting to 
     a decimal string, treat the integer `val` as a signed two's complement value 
     (this is how integers are stored in C). If the value is negative, you will 
     need to include a minus sign in the decimal string. When
     converting to a binary or hexidecimal string, treat the value as unsigned.
     In your conversions, you will likely need to do some division. Remember 
     that there is no division instruction so we can only divide by constants, 
     not variables.

     The `width` is the minimum number of characters in the returned string. 
     If `width` is more characters than needed to fit the formatted value, pad the 
     string with zeros on the left so that it has length `width`. If `width` is 
     fewer characters than needed to hold the formatted value, ignore the `width` 
     parameter.
  
     These functions should return a pointer to the start
     of the formatted string.
  
     Remember that it is easier to debug small steps than large changes, 
     so make sure these functions are working before you move on. You could test 
     these functions locally on your laptop by compiling using gcc and printing 
     the values returned by your functions using `printf` from the standard C library
     or by using gdb to examine the returned strings.

     Another option would be to do what is called *test-driven development*. You could 
     write tests first using `assert` and then write the code to pass the test. 
     You will be writing your own `assert` function later, so it would be worthwhile to 
     familiarize yourself with it by studying the man page now (`man assert`).
     For example, one test might look like 

     ```
     char* out = inttodec(buf, BUF_SIZE, 5, 4);
     assert(strcmp(out, "0005") == 0);
     ```

     This assert will fail until you correctly implement inttodec. Write other similar 
     tests for negative numbers and other unusual inputs. Each fix you make to the code 
     should just try to pass one more test. If your tests are well-designed, once you 
     are passing all tests, you know your code is working correctly!

     Once you have this part working, it would be a good time to commit your code 
     (using `git add` and `git commit` commands). 
     Then if something breaks later on, you can always return to this known-good state.

3.  Implement basic `snprintf`.

    snprintf(char *s, int n, char *format, ... );

    If you are unfamiliar with this function,
    take a look at its man pages: `man snprintf`.

    Your version need not have all the options of the standard version. 
    We will implement a simple version that handles the most common use cases.

    ```
    "%c" - print a char
    "%s" - print a string
    "%d" - print an int in decimal
    "%x" - print an int in hexadecimal
    "%b" - print an int in binary
    ```

    When implementing these functions, note that %d should treat the argument 
    as a signed number, and %x and %b should treat the argument as unsigned.
    This lines up with how you should have written your `inttodec`, 
    `inttohex`, and `inttobin` functions above -- so use your helper functions!

    The formatting codes for integers should optionally contain a width.
    For example, "%04x" would be a string of at least four characters (padded 
    with zeros on the left as necessary) representing the hexidecimal value.

    Again, remember that incremental steps are the key to success. First, 
    implement a version of snprintf that can handle string constants and 
    test it with simple examples such as `snprintf(buf, BUF_SIZE, "Hello World\n")`.
    Next, add the ability to handle chars and add tests such as 
    `snprintf(buf, BUF_SIZE, "%c = a\n", 'a')`. If you would like to compare your 
    `snprintf` outputs with the standard C `snprintf` outputs (using an
    `assert` for example), you will need to rename your version of `snprintf` 
    while debugging so 
    that it does not conflict with the standard library version.

    See the note below about how to create functions
    with variable numbers of arguments.

4.  Use your `snprintf` to implement `printf`,
    which should output characters to the mini uart using `uart_putc` from 
    uart.c. 

    printf(char *format, ... );

    You may notice that the `uart_init` function in uart.c calls your gpio_set_function 
    (from assignment 2) with a value of ALT5 (equal to 2). In
    assignment 2, your gpio_set_function code was only used to handle input and 
    output modes (values of 0 or 1). Ensure your gpio_set_function code handles all 
    alternate functions 0-5 as well. See page 92 of the [broadcom peripherals manual]
    (http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf) 
    for the codes for alternate functions, and page 102 for what these 
    alternate functions are for each pin. You can see that for pins 14 and 15 
    that we use for the UART, ALT5 sets these as TXD1 and RXD1.

    By now, you should be familiar with testing your code step by step. Hopefully you 
    have been writing mini-tests all along to verify your functions. 
    We are requiring that you 
    hand in some of your final tests to prove to us (and yourself!) that your code is 
    functioning correctly. 

    Copy the below main function, which uses your code to 
    print the sizes of the basic C types to your console.

    ```
    void main(void) { 
         while(1) { 
             printf("sizeof(char) = %d\n", sizeof(char) ); 
             printf("sizeof(char*) = %d\n", sizeof(char*) ); 
             printf("sizeof(short) = %d\n", sizeof(short) ); 
             printf("sizeof(short*) = %d\n", sizeof(short*) ); 
             printf("sizeof(int) = %d\n", sizeof(int) ); 
             printf("sizeof(int*) = %d\n", sizeof(int*) ); 
             printf("sizeof(long) = %d\n", sizeof(long) ); 
             printf("sizeof(long*) = %d\n", sizeof(long*) ); 
             printf("sizeof(long long) = %d\n", sizeof(long long) ); 
             printf("sizeof(long long*) = %d\n", sizeof(long long*) ); 
	     //my_tests();
             delay(2); 
        } 
    } 
    ```

    Now uncomment the call to my_tests and create this function with your own tests.
    You should include at least one 
    test for each type of printf argument (%c, %s, %b, %x) with and without 
    a width defined. For example, such tests might look like

    ```
    printf("%d = -5\n", -5);
    printf("%b = 101\n", 5);
    printf("%04x = 00AB\n", 0xab);
    ```

    Also make sure you add tests for unusual cases. What happens if you try 
    `printf("")`? What about `printf("\n")`? If you are ever unsure of what 
    should be printed, compile the test with the standard library printf 
    implementation and compare the output to your printf output.

5.  Implement `assert`, which should test if the given condition is true. 
    If not, it should print a useful error message (using your `printf`) 
    that includes the file and line number where the assert failed, and the 
    condition that failed. Also turn on the ACT LED on your 
    raspberry pi so that you can detect the failure even if your printf is 
    not working. After printing an error message and turning on 
    the LED, it should go into an infinite loop to effectively terminate 
    the running program.

    To get the file and line number, you will need to 
    use \_\_FILE\_\_ and \_\_LINE\_\_. These will print the file and line number of 
    the currently executing code. Unfortunately, if you put these macros 
    inside an `assert` function, they will print a line number inside the 
    `assert` function rather than the line where the assert was called. To 
    remedy this, you will need to implement `assert` as a macro so that the 
    preprocessor will replace the call to `assert` with its implementation.

    To print out the condition, look into
    [stringification](https://gcc.gnu.org/onlinedocs/cpp/Stringification.html).

#### Using variable numbers of arguments

`printf` is a function that takes a variable number of arguments.
C has standardized the way to access variadic functions 
using the `stdarg.h` interface. 
Here is an example.

    
    #include <stdarg.h>
    #include <stdio.h>

    int sum(int n, ...)
    {
        int sum = 0;
        va_list ap;

        va_start(ap, n);
        for( int i=0; i<n; i++ )
            sum += va_arg(ap, int);
        va_end(ap);

        return sum;
    }

    int main(void)
    {
        printf("%d\n", sum(3,  1,2,3) );
        return 0;
    }
    

Note how to declare a function with a variable number
of arguments using `...`.
The function `sum` has one required argument `n`,
and then a variable number of additional arguments.
This function should be called with `n` additional arguments.
For example, we call it from main as `sum(3, 1, 2, 3)`.
In general, functions require one argument.
It should be possible to determine the number of arguments
from the first argument.

The function `sum` shows how to use the `stdarg.h`.
First, we declare a variable `ap` as type `va_list`.
This variable is initialized using `va_start`.
Then we loop over the `n` arguments.
We fetch the next argument using `va_arg(ap, type)`.
The types can be different for each argument.
You need to know the type for this to compile correctly.
In this case, we assume all the arguments are of type `int`.
When we are done, we call `va_end(ap)` to clean up after ourselves.

For more details about `stdarg`, 
read the [Wikipedia Page stdarg.h](http://en.wikipedia.org/wiki/Stdarg.h).

### Extension

Implement a version of `scanf`. Check out the man page to learn 
about this function. You should implement this extension in a 
separate file scanf.c.

You will need to use the `uart_getc` function from uart.c to read 
input from the console (typed on your laptop keyboard). Whenever a 
return is pressed on the keyboard, you should process the received 
characters according to the given format string. You will need to 
convert from the ASCII codes output by the keyboard to the expected 
format. You should handle %c, %s, %d, and %x formats. Make sure you 
handle special cases such as negative numbers.

Write test code that scans in each of the format types (%c, %s, %d, %x) 
and then prints the character, string, or number back out to the 
screen using your `printf` function.
