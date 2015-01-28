## Assignment 3: Implementing a string formatting library

Often when programming embedded computers
space is at a premium.
Many of the C standard libraries are surprisingly large,
and it is useful to have simpler versions of standard functions available.

`printf` is particularly useful,
since printing is the most basic form of debugging.

The goals of this assignment are

- Understand C strings and characters.

- Understand how to convert basic C types (`int`, `int*`, ...) to a string.

- Use your gpio and timer libraries built in the previous assignment.

- Use the uart library shown in class to communicate with your laptop.

### Basic Part: Basic formatting functions

1. There is no starter code for this project. 
  Setup a project in the `assign3` directory in your course repository.
  Include README.md, Makefile, and your timer and gpio functions
  you used in the clock assignment. 

2. Implement the following basic formatting functions: 

  ```
    char* intobin( char *buf, int n, int val, int width); 
    char* intodec( char *buf, int n, int val, int width); 
    char* intohex( char *buf, int n, int val, int width); 
  ```
  In these functions, `buf` is a character array to store 
  the string representing the number. 
  The size of the buffer is equal to n.
  Do not write past the end of the buffer!
  
  `val` is the integer value to be converted to a string.
  
  `width` is the number of digits to be formatted. 
  If necessary, pad the string with spaces or zeros
  so that it has length `width`.
  
  These functions should return a pointer to the start
  of the formatted string.
  
3. Implement basic `snprintf`.

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

  The formatting codes for integers should optionally contain a width.
  For example, "%2x" will print the first two characters
  of the hexadecimal int.

  Use your `snprintf` to implement `printf`,
  which should output characters to the mini uart using `uart_putc`.

    printf(char *format, ... );

  See the note below about how to create functions 
  with variable numbers of arguments.

4. Use your code to print the sizes of the basic C types to your console:

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
             delay(2); 
        } 
    } 
  ```

### Extensions

Implement a markdown style table formatter.

     header("|Addr|Data|");
     for( i=0; i<n; i++ ) {
         row( "|%x|%x|", addr, *addr ):
         addr += 1;
     }
     footer();

The `header` defines the header for the table. 
Each column of the table is contained in between "|'.
This table will have two column headers, 
one with "Addr" and the other with "Data".
The `row` function creates a row in the table.
In this case, each row will contain two numbers,
an address and the value stored at that address.
This is formatted using the snprintf function you created 
in the basic part of the assignment.
Finally, the `footer` functions ends the table,
and prints the result.

The only tricky part of thie function is that
you need to compute the width of each column.
You wouldn't know that until all the rows have been formatted.
Thus, you will need to make two passes over the data.
The first pass will calculate the column widths of each row.
The actual column width will be the maximum width needed for any row.
The second pass will allocate space for each row,
and perform the final formatting.

Use these routines to perform a `hexdump` of memory.

    hexdump( int *addr, int n );

For example, 

    hexdump( 0x8000, 0x1000 );

should print out the `0x1000` words starting at location `0x8000`.

### Using variable numbers of arguments

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

