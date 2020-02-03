echo +++ CS107E local .gdbinit successfully loaded +++\n
# this breakpoint will trap call to uart_putchar and 
# re-route to output char in gdb
b uart_putchar
commands
silent
printf "%c", $r0
return
cont
end

target sim
load
