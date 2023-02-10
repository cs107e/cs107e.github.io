# CS107e gdb config file for use as local .gdbinit

echo Auto-loading commands from \033[32m .gdbinit\033[0m... \n

define uart-reroute
set breakpoint pending on
b uart_putchar
commands
silent
printf "%c", $r0
return
cont
end
end

document uart-reroute
set breakpoint on uart_putchar and re-route to local echo
end

define stop-at-hang
b hang
commands
echo \tSimulated program has run to completion\n
end
end

document stop-at-hang
set breakpoint on hang to announce when program has run to completion
end

# Execute default commands below for all programs

uart-reroute
stop-at-hang
target sim
load

