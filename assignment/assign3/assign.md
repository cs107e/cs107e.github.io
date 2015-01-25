## Assignment 3: Implementing a string formatting library

Unfortunately, when programming bare metal,
standard libraries are not available.
Many of the C standard libraries are also surprisingly large,
and it is useful to have simpler versions available.

`printf` is particularly useful,
since printing is the most universal and basic form of debugging.

The goals of this assignment are

- Understand C strings and characters.

- Understand how to convert basic C types (`int`, `int*`, ...)
to and from strings.
Converting from a string is done with `scan`;
converting to a string is done with `print`.

- Use your gpio and timer libraries built in the previous assignment.

- Use the uart library shown in class to communicate with your laptop.

### Basic Part: Basic formatting functions

1 There is no starter code for this project. 
Setup a project in the `assign3` directory in your course repository.
Include README.md, Makefile, and your timer and gpio functions
you used in the clock assignment.

2 Implement the following basic formatting functions: 

    int bintoint( char *buf );
    int dectoint( char *buf );
    int hextoint( char *buf );
    int atoi( char *buf );

    int intobin( char *buf, int n, int i, int width, int zero );
    int intodec( char *buf, int n, int i, int width, int zero );
    int intohex( char *buf, int n, int i, int width, int zero );
    int itoa( char *buf, int n, int i );

3 Implement basic versions of `sprintf` and `sscanf`.

    snprintf(char *s, int n, char *format, ... );
    snscanf(char *s, int n, char *format, ... );

If you are unfamiliar with these functions,
read the documentation at ...

Your versions need not have all the options of the
industrial strength versions. 
We want simple versions that handle the most common use cases.

    "%c" - print a char
    "%s" - print a string
    "%d" - print an int in decimal
    "%x" - print an int in hexadecimal
    "%b" - print an int in binary

The formatting codes for integers can contain a width.
For example, "%2x" will print the first two characters
of the hexadecimal int;
leading zeros are not printed for numbers.
"%02x" will always print the leading zeros.

Use `snprintf` to implement `printf`,
which should output characters to the mini uart using `uart_putc`.

    printf(char *format, ... );

See the note below about how to use functions with
variable numbers of arguments.

4 Use your code to print the sizes of the
basic C types to your console:

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


### Extensions

There are two possible extensions for this assignment.

1. Implement a markdown style table formatter.

     header("|Addr|Data|", "|----|----|" );
     for( i=0; i<n; i++ ) {
         row( "|%x|%x|", addr, *addr ):
         addr += 1;
     }
     footer();

Use these routines to perform a `hexdump` of memory.

2. Implement an additional formatting option "%A" that
disassembles an ARM machine language instruction to a string.
Do this for data processing instructions, loads and stores, and branches.
*This will be challenging!*


### Using variable numbers of arguments

....
