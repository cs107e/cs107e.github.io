---
title: "Guide: Using make for cross-development"
toc: true
attribution: Written by Julie Zelenski
---

## What is a Makefile?

A _Makefile_ is
a text file that describes the commands and options needed to build a program. The Makefile is used by the tool `make`.  As you will see soon enough, using a Makefile to automate the build process saves you a lot of retyping and makes your life
as a developer a whole lot smoother!

This guide uses example Makefiles written for a cross-development build. For information about Makefiles in other contexts,
refer to the resources in the section [Going Further](#resources).

## An example Makefile
The example makefile below builds the `blink.bin` program from the `blink.c` source file.
The labs and assignments use Makefiles very similar to this one,
so you will soon become familiar with the common structure.

```makefile
NAME = blink

ARCH    = -march=rv64im -mabi=lp64
CFLAGS  = $(ARCH) -g -Og -I$$CS107E/include -Wall -ffreestanding
LDFLAGS = -nostdlib -L$$CS107E/lib -T memmap.ld

all: $(NAME).bin
 
%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@

%.elf: %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $< -o $@

%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

run: $(NAME).bin
	mango-run $<

clean:
	rm -f *.o *.elf *.bin
```

This Makefile may look a bit cryptic at first! Let's try breaking it down step by step.

## Rules and recipes

In lecture, Pat whipped up a simple "doit" script to automate retyping the commands to rebuild a program. The `make` tool is just a fancier version of doit. A `Makefile` is a text file that describes the steps needed to build a program.
Here is an example of a very simple hard-coded Makefile containing three targets `all`, `button.bin` and `clean`:
    
```makefile
all: button.bin

button.bin: button.c
	riscv64-unknown-elf-gcc -ffreestanding -c button.c -o button.o
	riscv64-unknown-elf-ld -nostdlib button.o -o button.elf
	riscv64-unknown-elf-objcopy button.elf -O binary button.bin
    
clean: 
	rm -f *.bin *.o
```

__Rules__ are written in the following way: "the __dependencies__ on the right-hand-side are required
to make the __target__ on the left-hand-side." Thus the first line
indicates that `button.bin` is required to make `all`. In other words, to make `all`, we must first make `button.bin`.
```makefile
    all: button.bin
```

This brings us to the next rule for `button.bin`:
```makefile
button.bin: button.c
	riscv64-unknown-elf-gcc -ffreestanding -c button.c -o button.o
	riscv64-unknown-elf-ld -nostdlib button.o -o button.elf
	riscv64-unknown-elf-objcopy button.elf -O binary button.bin
```
The ingredients (dependencies on the right-hand-side) are needed as the starting point to create the desired output (target on the left-hand-side). The indented lines that follow the rule are the commands that turn the ingredients into the final product. These steps are collectively called the __recipe__. Thus, in order to make `button.bin`, we start with our ingredient (`button.c`) and then step through the commands in the recipe.

We could add a comment to explain the additional flags included when invoking the compiler. Lines starting with `#` are treated as comments.
```makefile
# Build flag used:
#  -ffreestanding  generate code assuming environment is unhosted
#  -nostdlib       no link with standard libraries
```

The final rule indicates what should happen when we `make clean`; the recipe for the clean target removes any previous build products so the next compile starts fresh.
```makefile
clean: 
	rm -f *.bin *.o
```

One particularly nifty thing `make` is that only rebuilds a target when one or more of the components it depends on has changed.  If you attempt to re-build a target which is already up-to-date, `make` will tell you:
```console
$ make
make: Nothing to be done for `all'.
```

## Macros

After repeatedly copy-pasting the example Makefile to create a version for a new program, you can see the value in structuring it to be more general-purpose. After all, Makefiles are written for convenience! 

```makefile
NAME = blink
CFLAGS  = -ffreestanding
LDFLAGS = -nostdlib

all: $(NAME).bin

$(NAME).bin: $(NAME).c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $(NAME).c -o $(NAME).o
	riscv64-unknown-elf-ld $(LDFLAGS) $(NAME).o -o $(NAME).elf
	riscv64-unknown-elf-objcopy $(NAME).elf -O binary $(NAME).bin

clean: 
	rm -f *.bin *.o
```
We've added three __macros__ up top. They're similar to variables
in that they replace instances of the macro throughout the file with their assigned text.
The `$(macro_name)` syntax is used to access the value of the macro.
This makes it easy to change the name for a new program.

## Pattern rules
We can further generalize our Makefile by using _pattern rules_ that can be used to operate on any source file, without hard-coding to a particular name.

```makefile
# This pattern rule compiles a C program into an object file.
# filename.o is built from filename.c
%.o: %.c
	riscv64-unknown-elf-gcc $(CFLAGS) -c $< -o $@

# This pattern rule converts assembly instructions into an object file.
# filename.o is built from filename.s
%.o: %.s
	riscv64-unknown-elf-as $(CFLAGS) $< -o $@

# This pattern rule links an object file into an executable ELF file.
# filename.elf is built from filename.o
%.elf: %.o
	riscv64-unknown-elf-ld $(LDFLAGS) $< -o $@

# This pattern rule extract binary from an ELF executable
# filename.bin is built from filename.elf
%.bin: %.elf
	riscv64-unknown-elf-objcopy $< -O binary $@
```

The symbols that begin with `$` and `%` in a pattern rule are handled by `make` using the following interpretations:

* `%` is a wildcard symbol when used in a rule; `%.o` for example matches any file that ends with `.o`
* `$@` refers to the left part of the rule, before the `:`
* `$<` refers to the first element in the right part of the rule, after the `:`

One more special variable `$^` refers to all elements in the right part of the rule, after the `:`, which is to say all of the dependencies.

For further convenience, we can add a rule for the `run` target to load and execute our newly-built program on the Pi.

```makefile
run: $(NAME).bin   # load and execute binary program on Pi
	mango-run $<
```

With that finishing touch, you have a general Makefile that can be easily re-purposed for other projects. Now that you know that a Makefile is just a cookbook that culminates in the tasty program you wish to create, you're ready to add your favorite recipes and bon appetit!

<a name="tab"></a>
> __Make is failing with a cryptic error about `Makefile: *** missing separator`. What gives?__
In what is widely considered one of the dumber decisions in the history of computing, a Makefile distinguishes between tabs and spaces. The recipe lines for a target must be indented by a tab and an equivalent number of spaces just won't do. Edit your makefile and replace those errant spaces with a tab to restore Makefile joy.
{: .callout-warning}


## Going further
<a name="resources"></a>

Some follow up references on Makefiles:
- [a Makefile tutorial](http://www.opussoftware.com/tutorial/TutMakefile.htm)
- [another Makefile tutorial](http://www.delorie.com/djgpp/doc/ug/larger/makefiles.html)
- [CS107 Guide to Makefiles](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/guide/make.html)
- An inexhaustible source of make wisdom is the full manual for [GNU make](https://www.gnu.org/software/make/manual/html_node/index.html) which will tell you more that you could ever want to know.
- Reading makefiles from real world projects is a good way to see make in action.  A [search for makefile on github.com](https://github.com/search?q=makefile) will turn up a treasure trove.

