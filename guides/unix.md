---
title: Guide to the Unix command line
---

*Prepared by Alexandre Becker, cs107 TA, Modified for cs107E by Pat Hanrahan*

This course will use Unix for all the assignments. An important goal
of the course is to familiarize you with the Unix programming
environment. This includes the command line, text editors, and program
development tools.

To begin, you should be familiar with the *command line*.

Are you familiar with the following commands?

- Do you know how to open a terminal window that is running a
   *shell*?
- What are the commands for changing directories and listing files in a
   directory? Do you know how the filesystem is organized in Unix?
- Do you know how to create, move, rename, and delete files and directories
   from the command line?
- Do you know how to run a command with different options? Do you know how to
   find documentation about the options to a Unix command?
- Do you know how to create a text file using a Unix text editor such as vi or emacs? If not, you
   should learn one of these editors. CS107 has some info on
   [emacs](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/topics/emacs) and
   [vi](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/topics/vim).
- Do you know about environment variables such as `PATH`?
   Do you know the purpose of `PATH`? Do you know how to change `PATH`?

If you don't know how to do these things, you will need to learn these skills.

Philip Guo, a former PhD student at Stanford, produce some short
[videos](http://pgbovine.net/command-line-tutorial.htm) that introduce basic
unix commands. The CS107 course maintains an extensive collection of [unix reference documents and videos](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/) that may be helpful to you.

## Unix commands

This tutorial walks you through the use of most basic Unix commands for navigating the file system, viewing files, and reading the manual pages.

### Browsing the file system

Open a terminal. The terminal window will show:

    $

What does this mean? The `$` is called the "command prompt".
(Depending on what flavor of unix and your login profile,
you may see a different prompt.)

Let's try running our first command. 
Enter the `ls` command and then press the return key (or "enter" key):

    $ ls
    Documents  Downloads   Mail

What does this command do? It lists all the files and folders contained in the current directory. To include more details about each file, such as the type or the size, you can add the `-l` flag to the command:

    $ ls -l

Then, you may need to change the current directory. Use the `cd` command to set a different directory as current:

    $ cd Documents

Typing `cd` by itself, returns you to your home directory (the root
of your personal file system).
Wherever you are in the filesystem, 
you can print the full path of the current directory with the `pwd` command.

    $ cd
    $ pwd
    /Users/hanrahan
    
Congratulations, you can now browse and display the files 
wherever you have access on your machine!

### Creating folders and files

You may want to create a folder dedicated to cs107. 
To do that, go in the folder where you want to create a directory,
and use the `mkdir` command:

    $ mkdir Class
    $ cd Class
    $ ls
    Class
    $ mkdir cs107
    $ ls
    cs107

You can create a new file 'foo.c' using your favorite editor (Emacs or Vim). 

You can move the file to another directory with the `mv source destdir` command.

    $ mv foo.c cs107

If destination is a file, the `mv source destination` command will rename the file:

    $ cd cs107
    $ ls
    foo.c
    $ mv foo.c bar.c
    $ ls
    bar.c

You can also use the `cp source destination` command to make a copy of the file with a new name.

    $ cp bar.c foo.c
    $ ls
    bar.c foo.c

To delete folders and fies, use these commands:

+ `rm file` will delete a file
+ `rmdir folder` will delete a folder, if it is empty
+ `rm -r folder` will delete a folder and all the files or subdirectories that it contains

### Viewing a file

Besides opening a file in your favorite text editor (e.g. Emacs or Vim), 
you can view its contents with `cat file` or `more file`. 
Whereas  `cat` will print the entire file in one go,
`more` allows you to scroll up and down.
You can use these keystrokes to scroll:

+ __space__ to scroll down
+ __b__ to scroll up (b stands for "backward")
+ __q__ to quit
+ __/__ to open a search box at the bottom of the screen. After typing the word that you are looking for, its occurrences will be highlighted. Use __n__ and __p__ to jump to the next and previous occurrence of the word.

### Using the man pages

Each command has a manual page that shows you how to use it 
and what its options are.
You can view a man page using `man command`.
This is one of the most useful commands to know!

    $ man ls

Tip: you can use the same keyboard shortcuts to scroll the man page as in the `more` program!

Most commands also accept options to print out a short *usage* reminder.
For example,

    $ more --help

list all the options and keyboard shortcuts. 
If `--help` doesn't work, try `-h`.

### Tips

- Tired of manually `cd`-ing to where you need to be, or typing in
  lengthy paths by hand? If you're on a Mac, you can drag a file or
  folder from the Finder into your Terminal window to fill its path
  in.

- Did you accidentally `cd` into the wrong directory? Run `cd -` to go
  back to where you were.

- Use the Up arrow key at the command line to get a previous command,
  so you can rerun it, or edit it and then rerun it. Press Ctrl-R and
  type to search through previous commands you've entered.
