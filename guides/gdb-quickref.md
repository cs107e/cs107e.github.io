---
title: GDB quick reference
attribution: Prepared by Julie Zelenski
---

<style>
    .table td {
        margin: 0;
        padding: 0;
    }
    .table td:nth-child(1) {
        font-weight: 700;
        font-family: Inconsolata, Consolas, Menlo, monospace;
        color: #A60E13;
        width: 25%;
    }
    thead {
        font-weight: 700;
        font-size: 13pt;
        background-color: #eeeeee;
    }
</style>



### Opening gdb
Run gdb on the `program.elf` file that you wish to debug and specify command file `$CS107E/other/gdbsim.commands` to our custom common script to properly configure simulator.
```console
$ riscv64-unknown-elf-gdb --command=$CS107E/other/gdbsim.commands program.elf
Reading symbols from program.elf...
Auto-loading commands from CS107E/other/gdbsim.commands...
Connected to the simulator.
Loading section .text, size 0x510c lma 40000000
...
```

### Common GDB commands
The gdb debugger has a tremendous amount of features packed into it, allowing you to control program execution, observe and trace runtime behavior, examine and change registers and memory, and much more.  Below is a table of gdb commands for common actions. Make it your goal to get these basic commands under your belt now and plan to keep learning and extending your mastery from here. Becoming a wizard at using gdb is a major superpower for a programmer to have!

> __Pro-tip__ The table below lists the full name of the command but most can be abbreviated to save your overworked fingers. For example, `info registers` can be invoked as `i r`, `break` can be invoked `b`, `print/x` is shortened to `p/x`, and so on.
{: .callout-info}


| General |
|-|-|
|help | Help for gdb
| help CMD| Help for gdb command `CMD`
|⬆️ ⬇️|Scroll back/forward through gdb command history
|ctrl-r| Search backwards through gdb command history for a specific string
|quit | Quit gdb
{: .table .table-sm .commands }

| Run and stop program |
|-|-|
|run | Run program, start from beginning
|start | Run program, start from beginning, stop at entry to `main()`
|ctrl-c |  Interrupt executing program, give control back to gdb
|kill | Kill execution of program being debugged
{: .table .table-sm .commands }

| Breakpoints |
|-|-|
|break WHERE | Set breakpoint at `WHERE`. Examples:
|&nbsp;&nbsp;  break sum | Set breakpoint at function
|&nbsp;&nbsp;  break 13 | Set breakpoint at line number
|&nbsp;&nbsp;  break *0x40001234  | Set breakpoint at address of instruction
|info break | List all breakpoints
|delete NUM | Delete breakpoint `NUM`. With no argument deletes all.
{: .table .table-sm .commands }

| Executing |
|-|-|
|step | Execute one line of C source, step __into__ function call
|next | Execute one line of C source, step __over__ function call
|stepi | Execute one assembly instruction
|nexti  | Execute one assembly instruction, step over `jal` (function call)
| | Note: All of above accept argument N to repeat N actions, i.e. `step 5`
|until LINE | Execute up to line number `LINE`
|finish | Execute up to end of current function
|continue | Continue execution
{: .table .table-sm .commands }

| Registers |
|-|-|
|info registers | Show info about all registers
|print $a0  | Can access individual register by `$name`
|set $a0 = 8 | Set register's value
{: .table .table-sm .commands }

| Stack |
|-|-|
|backtrace | Show current stack backtrace
|info frame | Show info about current stack frame
|up | Select stack frame that called this one
|down | Select stack frame called by this one
{: .table .table-sm .commands }

| Viewing code |
|-|-|
|list | Show C source for currently executing function
|&nbsp;&nbsp; list sum | Show C source for named function
|&nbsp;&nbsp; list 45 | Show C source around line number
|disassemble | Show disassembly of currently executing function
|&nbsp;&nbsp; disassemble sum | Show disassembly of named function
|&nbsp;&nbsp; disassemble 0x40001234 | Show disassembly of instruction at address
|print/x $pc | Print program counter in hex
{: .table .table-sm .commands }

| Print |
|-|-|
|print EXPR | Evaluate `EXPR`, print result
|&nbsp;&nbsp; print/d *ptr | Evaluate and print result in decimal
|&nbsp;&nbsp; print/x (val & 0xff) | Evaluate and print result in hex
|&nbsp;&nbsp;  print/t mask | Evaluate and print result in binary
|display EXPR | Auto-print `EXPR` each time execution stops
|&nbsp;&nbsp;  display/d i | Auto-print uses same format modifiers as print above
|info display | List all auto-printed expressions
|undisplay NUM | Delete auto-printed expression `NUM`.  With no argument deletes all.
{: .table .table-sm .commands }

| Examine memory |
|-|-|
|x/FMT ADDR | Examine memory starting at address `ADDR`. `FMT` is repeat count, format letter, and size letter.
| |Size letters: `b` 1-byte, `h` 2-byte, `w` 4-byte, `g` 8-byte
| |Format letters: `d` decimal, `x` hex, `t` binary, `c`char, `s` string, `i` instruction
|&nbsp;&nbsp;  x/1gx $sp | Examine one 8-byte value, show in hex, address is top of stack
|&nbsp;&nbsp;  x/2wt 0x02000030 | Examine two 4-byte values, show in binary
|&nbsp;&nbsp;  x/10bc ptr | Examine ten 1-byte values, show as ASCII
{: .table .table-sm .commands }

## Additional resources

- CS107's [guide to gdb](https://cs107.stanford.edu/resources/gdb.html) is a good introduction.
- Watch Chris Gregg's [video tutorial on gdb](https://www.youtube.com/watch?v=uhIt8YqtmuQ&feature=youtu.be).
-  Looking to learn some fancier tricks? See these articles Julie wrote for a
programming journal: [Breakpoint Tricks](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump1.pdf)
and [gdb's Greatest Hits](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/resources/gdb_coredump2.pdf).
- Last but not least, the full online gdb manual tells all:
[http://sourceware.org/gdb/current/onlinedocs/gdb/index.html](http://sourceware.org/gdb/current/onlinedocs/gdb/index.html).
